/* chatsrv.cpp */
#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <stdio.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <pthread.h>

using namespace std;               
               
/* #define's */
#define LISTEN_PORT     5296
#define MAX_LINE_BUFF   1024

/* Structures */
struct client_t {
    bool opstatus;
    bool kickflag;
    vector<string> outbound;
};

struct cmd_t {
    string command;
    string op1;
    string op2;
};

/* Globals */
pthread_mutex_t room_topic_mutex;
string room_topic;
pthread_mutex_t client_list_mutex;
map<string, client_t> client_list;

/* Forward declarations */
void* thread_proc(void *arg);
int readLine(int sock, char *buffer, int buffsize);
cmd_t decodeCommand(const char *buffer);
int join_command(const cmd_t &cmd, string &msg);
int msg_command(const cmd_t &cmd, const string &nickname, string &msg);
int pmsg_command(const cmd_t &cmd, const string &nickname, string &msg);
int op_command(const cmd_t &cmd, const string &nickname, string &msg);
int kick_command(const cmd_t &cmd, const string &nickname, string &msg);
int topic_command(const cmd_t &cmd, const string &nickname, string &msg);
int quit_command(const string &nickname, string &msg);

/* main */
int main(int argc, char *argv[])
{
    struct sockaddr_in sAddr;
    int listensock;
    int newsock;
    int result;
    pthread_t thread_id;
    int flag = 1;
    
    listensock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    setsockopt(listensock, IPPROTO_TCP, SO_REUSEADDR, (char *) &flag, sizeof(int));

    sAddr.sin_family = AF_INET;
    sAddr.sin_port = htons(LISTEN_PORT);
    sAddr.sin_addr.s_addr = INADDR_ANY;

    result = bind(listensock, (struct sockaddr *) &sAddr, sizeof(sAddr));
    if (result < 0) {
        perror("chatsrv");
        return 0;
    }

    result = listen(listensock, 5);
    if (result < 0) {
        perror("chatsrv");
        return 0;
    }
    
    pthread_mutex_init(&room_topic_mutex, NULL);
    pthread_mutex_init(&client_list_mutex, NULL);

    while (1) {
        newsock = accept(listensock, NULL ,NULL);
        result = pthread_create(&thread_id, NULL, thread_proc, (void *) newsock);
        if (result != 0) {
            printf("could not create thread.\n");
            return 0;
        }
        pthread_detach(thread_id);
        sched_yield();
    }

    return 1;
}

void* thread_proc(void *arg)
{
  int sock;
  char buffer[MAX_LINE_BUFF];
  int nread;
  int flag = 1;
  bool joined = false;
  bool quit = false;
  string nickname;
  struct cmd_t cmd;
  string return_msg;
  map<string, client_t>::iterator client_iter;
  int status;
  string outstring;

  /* Retrieve socket passed to thread_proc */
  sock = (int) arg;
  /* Turn off Nagle's algorithm*/
  setsockopt(sock, IPPROTO_TCP, TCP_NODELAY, (char *) &flag, sizeof(int));
  /* Set socket to non-blocking */
  ioctl(sock, FIONBIO, (char *) &flag);
  
  while (!quit) {
      /* Get any commands that the client has sent */
      status = readLine(sock, buffer, MAX_LINE_BUFF);
      if (status < 0) {
          /* If we've lost the client then process it as a QUIT. */
          if (joined) {
              quit_command(nickname, return_msg);
          }
          return arg;
      } else if (status > 0) {
          cmd = decodeCommand(buffer);
          if (!joined && cmd.command != "JOIN") {
              return_msg = "203 DENIED - MUST JOIN FIRST";
          } else {
              if (cmd.command == "JOIN") {
                  if (joined) {
                      return_msg = "203 DENIED - ALREADY JOINED";
                  } else {
                      status = join_command(cmd, return_msg);
                      if (status > 0) {
                          joined = true;
                          nickname = cmd.op1;
                      }
                  }
              } else if (cmd.command == "MSG") {
                  msg_command(cmd, nickname, return_msg);
              } else if (cmd.command == "PMSG") {
                  pmsg_command(cmd, nickname, return_msg);
              } else if (cmd.command == "OP") {
                  op_command(cmd, nickname, return_msg);
              } else if (cmd.command == "KICK") {
                  kick_command(cmd, nickname, return_msg);
              } else if (cmd.command == "TOPIC") {
                  topic_command(cmd, nickname, return_msg);
              } else if (cmd.command == "QUIT") {
                  quit_command(nickname,return_msg);
                  joined = false;
                  quit = true;
              } else {
                  return_msg = "900 UNKNOWN COMMAND";
              }
          }
          return_msg += "\n";
          send(sock, return_msg.c_str(), return_msg.length(), 0);
      }
      /* Send any pending server commands to the client. */
      if (joined) {
          pthread_mutex_lock(&client_list_mutex);
          for (int i = 0; i < client_list[nickname].outbound.size(); i++) {
              outstring = client_list[nickname].outbound[i] + "\n";
              send(sock, outstring.c_str(), outstring.length(), 0);
          }
          client_list[nickname].outbound.clear();
          if (client_list[nickname].kickflag == true) {
              client_iter = client_list.find(nickname);
              if (client_iter != client_list.end()) {
                  client_list.erase(client_iter);
              }
              quit = true;
          }
          pthread_mutex_unlock(&client_list_mutex);
      }
      sched_yield();
  }
  close(sock);

  return arg;
}

int readLine(int sock, char *buffer, int buffsize)
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
    tv.tv_sec = 0;
    tv.tv_usec = 50;
    sockStatus = select(sock + 1, &fset, NULL,  &fset, &tv);
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

int join_command(const cmd_t &cmd, string &msg)
{
    int retval;
    map<string, client_t>::iterator client_iter;
    
    if (cmd.op1.length() == 0 || cmd.op2.length() > 0) {
        msg = "201 INVALID NICKNAME";
        return 0;
    } else {
        pthread_mutex_lock(&client_list_mutex);
        client_iter = client_list.find(cmd.op1);
        if (client_iter == client_list.end()) {
            if (client_list.size() == 0) {
                client_list[cmd.op1].opstatus = true;
            } else {
                client_list[cmd.op1].opstatus = false;
            }
            client_list[cmd.op1].kickflag = false;
            for (client_iter = client_list.begin(); client_iter != client_list.end(); ++client_iter) {
                /* Tell other clients that a new user has joined */
                if ((*client_iter).first != cmd.op1) {
                    (*client_iter).second.outbound.push_back("JOIN " + cmd.op1);
                }
                /* Tell the new client which users are already in the room */
                client_list[cmd.op1].outbound.push_back("JOIN " + (*client_iter).first);
                /* Tell the new client who has operator status */
                if ((*client_iter).second.opstatus == true) {
                    client_list[cmd.op1].outbound.push_back("OP " + (*client_iter).first);
                }
            }
            /* Tell the new client the room topic */
            pthread_mutex_lock(&room_topic_mutex);
            client_list[cmd.op1].outbound.push_back("TOPIC * " + room_topic);
            pthread_mutex_unlock(&room_topic_mutex);
            msg = "100 OK";
            retval = 1;
        } else {
            msg = "200 NICKNAME IN USE";
            retval = 0;
        }
        pthread_mutex_unlock(&client_list_mutex);
    }

    return retval;
}

int msg_command(const cmd_t &cmd, const string &nickname, string &msg)
{
    map<string, client_t>::iterator client_iter;
    
    pthread_mutex_lock(&client_list_mutex);
    for (client_iter = client_list.begin(); client_iter != client_list.end(); client_iter++) {
        (*client_iter).second.outbound.push_back("MSG " + nickname + " " + cmd.op1 + " " + cmd.op2);
    }
    pthread_mutex_unlock(&client_list_mutex);
    msg = "100 OK";
    
    return 1;
}

int pmsg_command(const cmd_t &cmd, const string &nickname, string &msg)
{
    map<string, client_t>::iterator client_iter;
    
    pthread_mutex_lock(&client_list_mutex);
    client_iter = client_list.find(cmd.op1);
    if (client_iter == client_list.end()) {
        msg = "202 UNKNOWN NICKNAME";
    } else {
        (*client_iter).second.outbound.push_back("PMSG " + nickname + " " + cmd.op2);
        msg = "100 OK";
    }
    pthread_mutex_unlock(&client_list_mutex);
    
    return 1;
}

int op_command(const cmd_t &cmd, const string &nickname, string &msg)
{
    map<string, client_t>::iterator client_iter;
    
    pthread_mutex_lock(&client_list_mutex);
    client_iter = client_list.find(nickname);
    if (client_iter == client_list.end()) {
        msg = "999 UNKNOWN";
    } else {
        if ((*client_iter).second.opstatus == false) {
            msg = "203 DENIED";
        } else {
            client_iter = client_list.find(cmd.op1);
            if (client_iter == client_list.end()) {
                msg = "202 UNKNOWN NICKNAME";
            } else {
                (*client_iter).second.opstatus = true;
                for (client_iter = client_list.begin(); client_iter != client_list.end(); client_iter++) {
                    (*client_iter).second.outbound.push_back("OP " + cmd.op1);
                }
                msg = "100 OK";
            }
        }
    }    
    pthread_mutex_unlock(&client_list_mutex);
    
    return 1;
}

int kick_command(const cmd_t &cmd, const string &nickname, string &msg)
{
    map<string, client_t>::iterator client_iter;
    
    pthread_mutex_lock(&client_list_mutex);
    client_iter = client_list.find(nickname);
    if (client_iter == client_list.end()) {
        msg = "999 UNKNOWN";
    } else {
        if ((*client_iter).second.opstatus == false) {
            msg = "203 DENIED";
        } else {
            client_iter = client_list.find(cmd.op1);
            if (client_iter == client_list.end()) {
                msg = "202 UNKNOWN NICKNAME";
            } else {
                (*client_iter).second.kickflag = true;
                for (client_iter = client_list.begin(); client_iter != client_list.end(); client_iter++) {
                    (*client_iter).second.outbound.push_back("KICK " + cmd.op1 + " " + nickname);
                }
                msg = "100 OK";
            }
        }
    }    
    pthread_mutex_unlock(&client_list_mutex);
    
    return 1;
}

int topic_command(const cmd_t &cmd, const string &nickname, string &msg)
{
    map<string, client_t>::iterator client_iter;
    
    pthread_mutex_lock(&client_list_mutex);
    client_iter = client_list.find(nickname);
    if (client_iter == client_list.end()) {
        msg = "999 UNKNOWN";
    } else {
        if ((*client_iter).second.opstatus == false) {
            msg = "203 DENIED";
        } else {
            pthread_mutex_lock(&room_topic_mutex);
            room_topic = cmd.op1;
            if (cmd.op2.length() != 0) {
                room_topic += " " + cmd.op2;
            }
            for (client_iter = client_list.begin(); client_iter != client_list.end(); client_iter++) {
                (*client_iter).second.outbound.push_back("TOPIC " + nickname + " " + room_topic);
            }
            pthread_mutex_unlock(&room_topic_mutex);
            msg = "100 OK";
        }
    }    
    pthread_mutex_unlock(&client_list_mutex);
    
    return 1;
}

int quit_command(const string &nickname, string &msg)
{
    map<string, client_t>::iterator client_iter;
    
    pthread_mutex_lock(&client_list_mutex);
    client_iter = client_list.find(nickname);
    if (client_iter == client_list.end()) {
        msg = "999 UNKNOWN";
    } else {
        client_list.erase(client_iter);
        for (client_iter = client_list.begin(); client_iter != client_list.end(); client_iter++) {
            (*client_iter).second.outbound.push_back("QUIT " + nickname);
        }
        msg = "100 OK";
    }
    pthread_mutex_unlock(&client_list_mutex);
    
    return 1;
}
