/* chatcli.h */
#ifndef CHATCLI_H
#define CHATCLI_H

#include <string>

using namespace std;


/* #define's */
#define MAX_LINE_BUFF   1024


/* Structures */
struct cmd_t {
    string command;
    string op1;
    string op2;
};


/* Function Declarations */
int connectAndJoin(string host, int port, string nickname);
int readLine(int sock, char *buffer, int buffsize, int timeout);
cmd_t decodeCommand(const char *buffer);


#endif
