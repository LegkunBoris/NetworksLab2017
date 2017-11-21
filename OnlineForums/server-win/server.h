#ifndef SERVER_H
#define SERVER_H

#include <winsock2.h>
#include <windows.h>
#include <ws2tcpip.h>
#include <stdio.h>
#include <stdint.h>

#define HELP    100
#define CREATE  200
#define QUIT    300
#define FORUMS  400
#define OPEN    500
#define NAME    600

#define MAXCLIENTS      5
#define MAXFORUMS       1000
#define BUFFLEN         512
#define MAXUSERSATFORUM 1000

struct user{
    int socket;
    char *name;
    char *ip;
    int port;
    int id;
    char *forum;

};

//struct HANDLERS{
//    HANDLE thread;
//    int users;
//    int users_id[MAXUSERSATFORUM];
//};

//struct HANDLERS handle[MAXFORUMS];

struct command{
    int _command;
    int _key;
};

typedef struct args_tag {
    int socket;
    char *ip;
    int port;
} args_t;

struct user users[MAXCLIENTS];
int usersOnline;
int nextUser;
args_t args;
int forumsCount;
char *forums[MAXFORUMS];
struct user ClearUser;

DWORD WINAPI connection_handler(void *args);
char* concatStrings(char *dist, char *source);
char* readfromfile(const char *path);

#endif // SERVER_H
