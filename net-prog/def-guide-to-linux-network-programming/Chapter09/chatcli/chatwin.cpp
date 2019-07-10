/* chatwin.cpp */
#include <sys/socket.h>
#include <qvbox.h>
#include <qpushbutton.h>
#include <qmessagebox.h>
#include <qapplication.h>
#include <qinputdialog.h>
#include "chatcli.h"
#include "chatwin.h"


cChatWin::cChatWin() : QMainWindow(0, "", WDestructiveClose)
{
    this->setCaption("Chat Client");

    QVBox *main = new QVBox(this);
    main->setSpacing(3);
    main->setMinimumHeight(300);
    main->setMinimumWidth(400);
    this->setCentralWidget(main);

    QHBox *row1 = new QHBox(main);
    row1->setMinimumHeight(280);
    chatEdit = new QTextEdit(row1);
    chatEdit->setMinimumWidth(300);
    chatEdit->setReadOnly(true);
    userList = new QListBox(row1);
    userList->setMinimumWidth(100);
    userList->setMaximumWidth(100);

    QHBox *row2 = new QHBox(main);
    msgEdit = new QLineEdit(row2);
    msgEdit->setMinimumWidth(250);
    QPushButton *sendButton = new QPushButton("&Send", row2);
    this->connect(sendButton, SIGNAL(clicked()), this, SLOT(sendButtonClicked()));
    QPushButton *privButton = new QPushButton("Send &Private", row2);
    this->connect(privButton, SIGNAL(clicked()), this, SLOT(privButtonClicked()));
    QPushButton *opButton = new QPushButton("&Op", row2);
    this->connect(opButton, SIGNAL(clicked()), this, SLOT(opButtonClicked()));
    QPushButton *kickButton = new QPushButton("&Kick", row2);
    this->connect(kickButton, SIGNAL(clicked()), this, SLOT(kickButtonClicked()));
    QPushButton *topicButton = new QPushButton("&Topic", row2);
    this->connect(topicButton, SIGNAL(clicked()), this, SLOT(topicButtonClicked()));
    QPushButton *quitButton = new QPushButton("&Quit", row2);
    this->connect(quitButton, SIGNAL(clicked()), this, SLOT(quitButtonClicked()));

    theTimer = new QTimer(this);
    this->connect(theTimer, SIGNAL(timeout()), this, SLOT(timerFired()));
    theTimer->start(250, true);
}

cChatWin::~cChatWin()
{
}

void cChatWin::sendButtonClicked()
{
    string send_string;
    char buffer[MAX_LINE_BUFF];
    cmd_t cmd;
    int status;

    if (msgEdit->text() == "") {
        return;
    }

    send_string = "MSG " + string::basic_string(msgEdit->text().ascii()) + "\n";
    send(client_socket, send_string.c_str(), send_string.length(), 0);
    status = readLine(client_socket, buffer, MAX_LINE_BUFF, 0);
    if (status < 0) {
        QMessageBox::critical(NULL, "Lost Connection", "The server has closed the connection.");
        this->close();
        return;
    }
    cmd = decodeCommand(buffer);
    if (cmd.command != "100") {
        QMessageBox::critical(NULL, "Unknown Error", "An unknown error has occurred.");
        return;
    }

    msgEdit->setText("");
}

void cChatWin::privButtonClicked()
{
    string send_string;
    char buffer[MAX_LINE_BUFF];
    cmd_t cmd;
    int status;
    string username;

    if (msgEdit->text() == "") {
        return;
    }

    if (userList->currentText() == "") {
        QMessageBox::critical(NULL, "Private Message", "You must select a user before sending a private message.");
        return;
    }
    
    username = userList->currentText().ascii();
    if (username[0] == '@') {
        username = username.substr(1);
    }

    send_string = "PMSG " + username + " " + string::basic_string(msgEdit->text().ascii()) + "\n";
    send(client_socket, send_string.c_str(), send_string.length(), 0);
    status = readLine(client_socket, buffer, MAX_LINE_BUFF, 0);
    if (status < 0) {
        QMessageBox::critical(NULL, "Lost Connection", "The server has closed the connection.");
        this->close();
        return;
    }
    cmd = decodeCommand(buffer);
    if (cmd.command == "100") {
        msgEdit->setText("");
        return;
    } else if (cmd.command == "202") {
        QMessageBox::critical(NULL, "Unknown User", "The user specified is not in the room.");
        return;
    } else {
        QMessageBox::critical(NULL, "Unknown Error", "An unknown error has occurred.");
        return;
    }
}

void cChatWin::opButtonClicked()
{
    string send_string;
    char buffer[MAX_LINE_BUFF];
    cmd_t cmd;
    int status;
    string username;

    if (userList->currentText() == "") {
        QMessageBox::critical(NULL, "Op Error", "You must select a user before making them an operator.");
        return;
    }
    
    username = userList->currentText().ascii();
    if (username[0] == '@') {
        username = username.substr(1);
        QMessageBox::critical(NULL, "Op Error", "User is already a room operator.");
        return;
    }

    send_string = "OP " + username + "\n";
    send(client_socket, send_string.c_str(), send_string.length(), 0);
    status = readLine(client_socket, buffer, MAX_LINE_BUFF, 0);
    if (status < 0) {
        QMessageBox::critical(NULL, "Lost Connection", "The server has closed the connection.");
        this->close();
        return;
    }
    cmd = decodeCommand(buffer);
    if (cmd.command == "100") {
        return;
    } else if (cmd.command == "202") {
        QMessageBox::critical(NULL, "Unknown User", "The user specified is not in the room.");
        return;
    } else if (cmd.command == "203") {
        QMessageBox::critical(NULL, "Denied", "Only room operators may op other users.");
        return;
    } else {
        QMessageBox::critical(NULL, "Unknown Error", "An unknown error has occurred.");
        return;
    }
}

void cChatWin::kickButtonClicked()
{
    string send_string;
    char buffer[MAX_LINE_BUFF];
    cmd_t cmd;
    int status;
    string username;

    if (userList->currentText() == "") {
        QMessageBox::critical(NULL, "Kick Error", "You must select a user before kicking them out.");
        return;
    }
    
    username = userList->currentText().ascii();
    if (username[0] == '@') {
        username = username.substr(1);
    }

    send_string = "KICK " + username + "\n";
    send(client_socket, send_string.c_str(), send_string.length(), 0);
    status = readLine(client_socket, buffer, MAX_LINE_BUFF, 0);
    if (status < 0) {
        QMessageBox::critical(NULL, "Lost Connection", "The server has closed the connection.");
        this->close();
        return;
    }
    cmd = decodeCommand(buffer);
    if (cmd.command == "100") {
        return;
    } else if (cmd.command == "202") {
        QMessageBox::critical(NULL, "Unknown User", "The user specified is not in the room.");
        return;
    } else if (cmd.command == "203") {
        QMessageBox::critical(NULL, "Denied", "Only room operators may kick out other users.");
        return;
    } else {
        QMessageBox::critical(NULL, "Unknown Error", "An unknown error has occurred.");
        return;
    }
}

void cChatWin::topicButtonClicked()
{
    string send_string;
    char buffer[MAX_LINE_BUFF];
    cmd_t cmd;
    int status;
    bool ok;

    QString topic = QInputDialog::getText("Chat Client", "Enter the new topic:", QLineEdit::Normal,
					  QString::null, &ok, this );
    if (ok == false || topic.isEmpty()) {
      return;
    }
    
    send_string = "TOPIC " + string::basic_string(topic.ascii()) + "\n";
    send(client_socket, send_string.c_str(), send_string.length(), 0);
    status = readLine(client_socket, buffer, MAX_LINE_BUFF, 0);
    if (status < 0) {
      QMessageBox::critical(NULL, "Lost Connection", "The server has closed the connection.");
      this->close();
      return;
    }
    cmd = decodeCommand(buffer);
    if (cmd.command == "100") {
      return;
    } else if (cmd.command == "203") {
      QMessageBox::critical(NULL, "Denied", "Only room operators may change the topic.");
      return;
    } else {
      QMessageBox::critical(NULL, "Unknown Error", "An unknown error has occurred.");
      return;
    }
    
}

void cChatWin::quitButtonClicked()
{
    string send_string;
    char buffer[MAX_LINE_BUFF];
    cmd_t cmd;
    int status;

    send_string = "QUIT\n";
    send(client_socket, send_string.c_str(), send_string.length(), 0);
    status = readLine(client_socket, buffer, MAX_LINE_BUFF, 0);
    if (status < 0) {
        QMessageBox::critical(NULL, "Lost Connection", "The server has closed the connection.");
        this->close();
        return;
    }
    cmd = decodeCommand(buffer);
    if (cmd.command != "100") {
        QMessageBox::critical(NULL, "Unknown Error", "An unknown error has occurred.");
    }

    this->close();
}

void cChatWin::timerFired()
{
    int status;
    char buffer[MAX_LINE_BUFF];
    cmd_t cmd;
    string str;

    /* Get any commands that the server has sent */
    while ((status = readLine(client_socket, buffer, MAX_LINE_BUFF, 100)) != 0) {
      qApp->processEvents();
        if (status < 0) {
            QMessageBox::critical(NULL, "Lost Connection", "The server has closed the connection.");
            this->close();
            return;
        } else if (status > 0) {
            cmd = decodeCommand(buffer);
            if (cmd.command == "JOIN") {
                userList->insertItem(cmd.op1.c_str());
                str = cmd.op1 + " has joined the room.\n";
            } else if (cmd.command == "MSG") {
                str = cmd.op1 + ": " + cmd.op2 + "\n";
                chatEdit->append(str.c_str());
            } else if (cmd.command == "PMSG") {
                str = cmd.op1 + " has sent you a private message:\n\n";
                str += cmd.op2;
                QMessageBox::information(NULL, "Private Message", str.c_str());
            } else if (cmd.command == "OP") {
                QListBoxItem *itm = userList->findItem(cmd.op1.c_str(), Qt::ExactMatch);
                if (itm != NULL) {
                    userList->changeItem("@" + itm->text(), userList->index(itm));
                }
                str = cmd.op1 + " has been made a room operator.\n";
                chatEdit->append(str.c_str());
            } else if (cmd.command == "KICK") {
                QListBoxItem *itm = userList->findItem(cmd.op1.c_str(), Qt::ExactMatch);
                if (itm != NULL) {
                    userList->removeItem(userList->index(itm));
                } else {
                    str = "@" + cmd.op1;
                    itm = userList->findItem(str.c_str(), Qt::ExactMatch);
                    if (itm != NULL) {
                        userList->removeItem(userList->index(itm));
                    }
                }              
                str = cmd.op1 + " was kicked out of the room by " + cmd.op2 + "\n";
                chatEdit->append(str.c_str());
            } else if (cmd.command == "TOPIC") {
                if (cmd.op1 != "*") {
                    str = "The topic has been changed to \"" + cmd.op2 + "\" by " + cmd.op1 + "\n"; 
                    chatEdit->append(str.c_str());
                }
                str = "Chat Client - Topic: " + cmd.op2;
                this->setCaption(str.c_str());
            } else if (cmd.command == "QUIT") {
                QListBoxItem *itm = userList->findItem(cmd.op1.c_str(), Qt::ExactMatch);
                if (itm != NULL) {
                    userList->removeItem(userList->index(itm));
                } else {
                    str = "@" + cmd.op1;
                    itm = userList->findItem(str.c_str(), Qt::ExactMatch);
                    if (itm != NULL) {
                        userList->removeItem(userList->index(itm));
                    }
                }
                str = cmd.op1 + " has left the room.\n";
                chatEdit->append(str.c_str());
            }
	}
    }
    theTimer->start(250, true);
}
