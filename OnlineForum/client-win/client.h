#ifndef CLIENT_H
#define CLIENT_H
#include <stdio.h>
#include <winsock2.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#define HELP 2090324718
#define RECON 273088444
#define QUIT 2090665480
#define true 1
int readn(int *socket,char* output,int mode);
const unsigned long hash(const char *str);
int init(int argc , char *argv[]);
int sendmsg(int *csocket,char *msg);
void *connection_handler(void *args);
char* concatStrings(char *dist, char *source);
#endif // CLIENT_H
