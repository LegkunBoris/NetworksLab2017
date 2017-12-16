 #ifndef SERVER_H
#define SERVER_H

#include <winsock2.h>
#include <windows.h>
#include <ws2tcpip.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#define HELP    100
#define CREATE  200
#define QUIT    300
#define FORUMS  400
#define OPEN    500
#define NAME    600
#define UNAME   601
#define ONLINE  700
#define DISPLAY 800
#define MAXCOUNT 10000
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
typedef struct argsTag {
    int socket;
    char *ip;
    int port;
} argsT;
struct registered{
    char *Name;
};

struct user users[MAXCLIENTS];
struct registered reged[MAXCLIENTS];
struct user ClearUser;
argsT args;
int usersOnline;
int registeredUsers;
int nextUser;
int forumsCount;
char *forums[MAXFORUMS];

DWORD WINAPI ConnectionHandler(void *args);
char* ConcatStrings(char *dist, char *source);
char* ReadFromFile(const char *path);

#endif // SERVER_H
