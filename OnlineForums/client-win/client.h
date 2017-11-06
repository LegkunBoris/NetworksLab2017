#ifndef CLIENT_H
#define CLIENT_H
#include <stdio.h>
#include <winsock2.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <stdint.h>
#define BUFFLEN 512
#define HELP    1
#define RECON   2
#define QUIT    3
#define FORUMS  4
#define OPEN    5
#define CREATE  6
int readn(int *socket,char* output,int mode);
int decodeString(char *str);
int init(int argc , char *argv[]);
void *connection_handler(void *args);
char* concatStrings(char *dist, char *source);
DWORD WINAPI chat_handler(void *args);

struct command{
    int _command;
    int _key;
};

int csocket;
#endif // CLIENT_H
