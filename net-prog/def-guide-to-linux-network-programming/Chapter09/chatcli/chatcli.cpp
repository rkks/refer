/* chatcli.cpp */
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/tcp.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <string>
#include <qapplication.h>
#include <qmessagebox.h>
#include "logindlg.h"
#include "chatwin.h"
#include "chatcli.h"

using namespace std;

/* Global Variables */
int client_socket;


int main(int argc, char **argv)
{
    cLoginDlg *logindlg;
    string host;
    int port;
    string nickname;

    QApplication app(argc, argv);

    do {
        logindlg = new cLoginDlg(NULL);
        if (logindlg->exec() == QDialog::Rejected) {
            delete logindlg;
            return 0;
        }
        host = logindlg->hostEdit->text().ascii();
        port = atoi(logindlg->portEdit->text().ascii());
        nickname = logindlg->nickEdit->text().ascii();
    } while (connectAndJoin(host, port, nickname) == 0);

    cChatWin *chatwin = new cChatWin();
    chatwin->client_socket = client_socket;
    app.setMainWidget(chatwin);
    chatwin->setCaption("Chat Test");
    chatwin->show();
    app.connect(&app, SIGNAL(lastWindowClosed()), &app, SLOT(quit()));
    int res = app.exec();
    return res;
}

int connectAndJoin(string host, int port, string nickname)
{
    sockaddr_in sAddr;
    hostent *dotaddr = NULL;
    int flag = 1;
    string joinString;
    char buff[MAX_LINE_BUFF];
    cmd_t cmd;

	client_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    bzero(&sAddr, sizeof(sockaddr_in));
    sAddr.sin_family = AF_INET;
    sAddr.sin_addr.s_addr = INADDR_ANY;
    sAddr.sin_port = htons(port);
    dotaddr = gethostbyname(host.c_str());
    if (dotaddr) {
        bcopy((const void *) dotaddr->h_addr_list[0], (void *) &sAddr.sin_addr.s_addr, dotaddr->h_length);
    } else {
        sAddr.sin_addr.s_addr = inet_addr("0.0.0.0");
    }

    /* Connect to server */
    if (connect(client_socket, (const sockaddr *) &sAddr, sizeof(sAddr)) != 0) {
        QMessageBox::critical(NULL, "Connection Failed", "Unable to connect.");
        close(client_socket);
        return 0;
    }

    /* Turn off Nagle's algorithm*/
    setsockopt(client_socket, IPPROTO_TCP, TCP_NODELAY, (char *) &flag, sizeof(int));
    /* Set socket to non-blocking */
    ioctl(client_socket, FIONBIO, (char *) &flag);

    joinString = "JOIN " + nickname + "\n";
    send(client_socket, joinString.c_str(), joinString.length(), 0);
    readLine(client_socket, buff, MAX_LINE_BUFF, 0);
    cmd = decodeCommand(buff);
    
    if (cmd.command == "100") {
        return 1;
    } else if (cmd.command == "200") {
        QMessageBox::critical(NULL, "Nickname In Use", "The nickname you've chosen is already in use.");
        close(client_socket);
        return 0;
    } else if (cmd.command == "201") {
        QMessageBox::critical(NULL, "Invalid Nickname", "The nickname you've chosen is invalid.");
        close(client_socket);
        return 0;
    } else {
        QMessageBox::critical(NULL, "Unknown Error", "An unknown error has occurred.");
        close(client_socket);
        return 0;
    }

}

int readLine(int sock, char *buffer, int buffsize, int timeout)
{
    char *buffPos;
    char *buffEnd;
    int nChars;
    int readlen;
    bool complete = false;
    fd_set fset;
    struct timeval tv;
    int sockStatus;
    int readSize;
    unsigned long blockMode;

    FD_ZERO(&fset);
    FD_SET(sock, &fset);
    if (timeout > 0) {
      tv.tv_sec = 0;
      tv.tv_usec = timeout;
      sockStatus = select(sock + 1, &fset, NULL,  &fset, &tv);
    } else {
      sockStatus = select(sock + 1, &fset, NULL,  &fset, NULL);
    }
    if (sockStatus <= 0) {
        return sockStatus;
    }

    buffer[0] = '\0';
    buffPos = buffer;
    buffEnd = buffer + buffsize;
    readlen = 0;
    while (!complete) {
        if ((buffEnd - buffPos) < 0) {
            readSize = 0;
        } else {
            readSize = 1;
        }
        FD_ZERO(&fset);
        FD_SET(sock, &fset);
        tv.tv_sec = 5;
        tv.tv_usec = 0;
        sockStatus = select(sock + 1, &fset, NULL,  &fset, &tv);
        if (sockStatus < 0) {
            return -1;
        }
        nChars = recv(sock, (char *) buffPos, readSize, MSG_NOSIGNAL);
        readlen += nChars;
        if (nChars <= 0) {
            return -1;
        } 
        if (buffPos[nChars - 1] == '\n') {
            complete = true;
            buffPos[nChars - 1] = '\0';
        }
        buffPos += nChars;
    }
    return readlen;
}

cmd_t decodeCommand(const char *buffer)
{
    struct cmd_t ret_cmd;
    int state;

    state = 0;
    for (int x = 0; x < strlen(buffer); x++) {
        if (buffer[x] == ' ' && state < 2) {
            state++;
        } else {
            switch (state) {
            case 0:     ret_cmd.command += toupper(buffer[x]);
                        break;
            case 1:     ret_cmd.op1 += buffer[x];
                        break;
            default:    ret_cmd.op2 += buffer[x];
            }
        }
    }

    return ret_cmd;
}
