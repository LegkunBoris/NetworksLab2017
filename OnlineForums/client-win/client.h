#ifndef CLIENT_H
#define CLIENT_H
#include <stdio.h>
#include <winsock2.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <stdint.h>

#define BUFFLEN 512
#define RECON   55
#define HELP    100
#define CREATE  200
#define QUIT    300
#define FORUMS  400
#define OPEN    500
#define NAME    600
#define UNAME   601
#define ONLINE  700
#define DISPLAY 800

int readn(int *socket,char* output,int mode);
int decodeString(char *str);
int init(int argc , char *argv[]);
void *connection_handler(void *args);
char* concatStrings(char *dist, char *source);
DWORD WINAPI chat_handler(void *args);

char name[80];
int csocket;
char *forum;
#endif // CLIENT_H
