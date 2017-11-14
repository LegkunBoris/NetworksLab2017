/*
    Create a TCP socket
*/

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

#define MAXCLIENTS  5
#define MAXFORUMS   1000
#define BUFFLEN     512

const char *OpenErrorCode = "Error opening file";
struct user{
    int socket;
    char *name;
    char *ip;
    int port;
    int id;
    char *forum;

};

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
int usersOnline = 0;
int nextUser = 0;
args_t args;
int forumsCount = 0;
static char *forums[MAXFORUMS];
static const struct user ClearUser;

DWORD WINAPI connection_handler(void *args);
char* concatStrings(char *dist, char *source);
char* readfromfile(const char *path);

int loadForums(){
    int count = 0;
    FILE *fp;
    if ((fp=fopen("forums.txt", "r") )==NULL) {
        printf("%s %d",OpenErrorCode,1);
        return 1;
    }
    char tmp[BUFFLEN];
    int num_chars = 0;
    int ch;
    while ((ch = fgetc(fp))!= EOF){
        if(ch != '\n')
            tmp[num_chars++] = ch;
        else{
            tmp[num_chars] = '\0';
            char *newString = (char *)malloc(num_chars);
            strcpy(newString,tmp);
            newString[strlen(newString)] = '\0';
            forums[count] = newString;
            printf("forums[%d] [%s] added\n",count,forums[count]);
            memset(tmp,0,strlen(tmp));
            num_chars = 0;
            count++;
        }
    }
    fclose(fp);
    return count;
}

int decodeString(char *str) {
    if(strcmp(str, "help") == 0)
        return HELP;
    else if(strcmp(str, "quit") == 0)
        return QUIT;
    else if(strcmp(str, "forums") == 0)
        return FORUMS;
    else if(strcmp(str, "open") == 0)
        return OPEN;
    else if(strcmp(str, "name") == 0)
        return NAME;
    else if(strcmp(str, "create") == 0)
        return CREATE;
    else
        return -1;
}
int main(int argc , char *argv[])
{
    WSADATA wsa;
    SOCKET serverSocket,accSocket;
    struct sockaddr_in server,client;
    char *answer = "I've got your message!";
    if (WSAStartup(0x0202,&wsa) != 0)
    {
        printf("Failed. Error Code : %d",WSAGetLastError());
        return 1;
    }
    //Create a socket
    if((serverSocket = socket(AF_INET , SOCK_STREAM , 0 )) == INVALID_SOCKET)
    {
        printf("Could not create socket : %d" , WSAGetLastError());
    }

    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_family = AF_INET;
    server.sin_port = htons( (UINT16)atoi(argv[1]) );

    //Bind
    if( bind(serverSocket ,(struct sockaddr *)&server , sizeof(server)) == SOCKET_ERROR)
    {
        printf("Bind failed with error code : %d" , WSAGetLastError());
    }
    //Listen to incoming connections
    listen(serverSocket , MAXCLIENTS);
    printf("Waiting for incoming connections...\n");
    int clilen = sizeof(struct sockaddr_in);
    forumsCount = loadForums();

    while( (accSocket = accept(serverSocket, (struct sockaddr *)&client, (socklen_t*)&clilen)) )
    {

        args.ip = inet_ntoa(client.sin_addr);
        args.port = ntohs(client.sin_port);
        args.socket = accSocket;
        HANDLE thread = CreateThread(NULL,0,connection_handler,&args,0,NULL);
        printf("Thread created for: %s; on Port: %d\n",inet_ntoa(client.sin_addr) ,ntohs(client.sin_port));
        usersOnline = usersOnline + 1;
        printf("Current online : %d\n",usersOnline);
    }

    if (accSocket < 0)
    {
        perror("accept failed");
        return 1;
    }

    closesocket(serverSocket);
    WSACleanup();
}
int readn(int socket,char** output){
    char buffer[BUFFLEN];
    int length = 0;
    char *tmp = "";
    int n;
    while(TRUE)
    {
        n = recv(socket, buffer, BUFFLEN,0);
        length += n;
        tmp = concatStrings(tmp,buffer);
        if(n < BUFFLEN)
            break;
    }
    tmp[length] = '\0';
    *output = tmp;
    return length;
}

/*
 * This will handle connection for each client
 * */
DWORD WINAPI connection_handler(void *args)
{
    args_t *arg = (args_t*) args;

    //Get the socket descriptor
    int csocket = arg->socket;
    int user_id = nextUser;
    users[user_id].socket = arg->socket;
    users[user_id].id = nextUser;
    users[user_id].ip = arg->ip;
    users[user_id].port = arg->port;
    users[user_id].forum = "";

    nextUser = nextUser + 1;
    while(TRUE){
        char *buffer;
        int len = 0;
start:  free(buffer);
        len = readn(csocket,&buffer);
        if(len == -1)
        {
           return;
        }

        if(!(strlen(buffer) != 0))
            goto start;
        printf("decoding [%s]\n",buffer);
        struct command com;
        char *token;
        char *rest = buffer;

        if(token = strtok_r(rest, " ", &rest))
            com._command = atoi(token);
        token = strtok_r(rest, " ", &rest);
        printf("int of command:[%d] string command:[%s] token:[%s]\n", com._command, token);

        switch(com._command){
         case CREATE:{
            printf("in create\n");
            for(int i = 0; i < forumsCount;i++)
            {
                if(forums[i] == token){
                    char msg[] = "Forums already exists!";
                    int n = send(csocket, msg, strlen(msg),0);
                    if (n < 0) {
                        perror("ERROR writing to socket\n");
                        break;
                    }
                }
            }
            printf("check on exist\n");
            void *newString = malloc(strlen(token));
            strcpy(newString,token);
            forums[forumsCount++] = newString;
            for(int i = 0; i < forumsCount;i++)
                 printf("forum[%d] [%s]\n",i,forums[i]);
            FILE *f = fopen("forums.txt","a");
            if (f == NULL)
            {
                printf("Error opening file!\n");
                break;
            }
            fprintf(f, token);
            fprintf(f, "\n");
            fclose(f);
            printf("Forum [%s] added to forum list.\n",token);
            char *file_name = concatStrings(token,".txt");
            f = fopen(file_name ,"w");
            if (f == NULL)
            {
                printf("Error opening file!\n");
                break;
            }
            fclose(f);
            printf("Forum file added.\n");
            break;
         }
         case HELP:  {
            char *msg = readfromfile("help.txt");
            int n = send(csocket, msg, strlen(msg),0);
            if (n < 0) {
                perror("ERROR writing to socket\n");
                return 0;
            }
            free(msg);
            printf("Help sent to user:[%s]\n",users[user_id].name);
            break;
         }
        case NAME:{
            char *name;
            len = readn(csocket,&name);
            users[user_id].name = name;
            printf("User [%s] loged in.\n",users[user_id].name);
            break;
        }
        case OPEN:{
            com._key = -1;
            for(int i=0;i < forumsCount;i++)
                if(strcmp(forums[i], token) == 0)
                    com._key = i;
            if(com._key != -1)
            {
                users[user_id].forum = forums[com._key];
                printf("User [%s] opened forum [%s]\n",users[user_id].name,users[user_id].forum);
                int n = send(csocket, users[user_id].forum, strlen(users[user_id].forum),0);
                if (n < 0) {
                    perror("ERROR writing to socket\n");
                    return 0;
                }
                char *file_name = concatStrings(token,".txt");
                char *msg = readfromfile(file_name);
                n = send(users[user_id].socket, msg, strlen(msg),0);
                if (n < 0) {
                    perror("ERROR writing to socket\n");
                    return 0;
                }
                while(TRUE)
                {
                    char *message;
                    int n = readn(csocket,&message);
                    if(n < 0)
                        break;
                    if(strcmp(message,"--quit") == 0)
                    {
                        users[user_id].forum = "";
                        break;
                    }
                    else
                    {
                        printf("Message [%s] from user:[%s]\n",message,users[user_id].name);
                        char *file_name = concatStrings(forums[com._key],".txt");
                        WriteMessageToFile(file_name,message);
                        for(int i = 0;i < usersOnline;i++)
                            if(strcmp(users[i].forum,token) == 0)
                            {
                                if(users[i].id != user_id)
                                {
                                    int n = send(users[i].socket,message, strlen(message),0);
                                    if (n < 0) {
                                        perror("ERROR writing to socket\n");
                                        return 0;
                                    }
                                }
                            }
                    }
                }
            }
            else
            {
                char *msg = "error";
                printf("Forum [%s] doesnt exist.\n",token);
                int n = send(csocket, msg, strlen(msg),0);
                if (n < 0) {
                    perror("ERROR writing to socket\n");
                    return 0;
                }
            }
            break;
        }
        case QUIT:{
            printf("Closing connection to [%s]\n",users[user_id].name);
            char msg[] = "quit";
            int n = send(csocket, msg, strlen(msg),0);
            if (n < 0) {
                perror("ERROR writing to socket\n");
                return 0;
            }
            usersOnline = usersOnline - 1;
            closesocket(users[user_id].socket);
            nextUser = user_id;
            users[user_id] = ClearUser;
            free(msg);
            closesocket(users[user_id].socket);
            return user_id;
        }
        case FORUMS:{
            int convert = htonl(forumsCount);
            int n = send(csocket, &convert, sizeof(convert),0); // send on Windows
            if (n < 0) {
                perror("ERROR writing to socket");
                exit(1);
            }
            for(int i = 0; i < forumsCount;i++)
            {
                int bytes = htonl(strlen(forums[i]));
                int n = send(csocket, &bytes, sizeof(bytes),0); // send on Windows
                if (n < 0) {
                    perror("ERROR writing to socket");
                    exit(1);
                }
                n = send(csocket, forums[i], strlen(forums[i]),0); // send on Windows
                if (n < 0) {
                    perror("ERROR writing to socket");
                    exit(1);
                }
                printf("[%s] sent\n",forums[i]);
            }
            printf("list of forums has been sent to user:[%s]\n",users[user_id].name);
            break;
        }
        default:    {
            printf("command [%s] not found\n",buffer);
            return 0;
         }
    }
    }
}
void WriteMessageToFile(char *path,char *data)
{
    FILE *f = fopen(path,"a");
    if (f == NULL)
    {
        printf("Error opening file!\n");
        return;
    }
    fprintf(f, data);
    fprintf(f, "\n");
    fclose(f);
}

char* readfromfile(const char *path)
{
    FILE *fp;
    if ((fp=fopen(path, "r") )==NULL) {
        printf("Cannot open file.\n");
        return 1;
    }
    char *msg = "";
    char tmp[256];
    while(fgets(tmp, sizeof tmp, fp)) {
        msg = concatStrings(msg,tmp);
    };
    printf(msg);
    fclose(fp);
    return msg;

}
char* concatStrings(char *dist, char *source){
    void *newString = malloc(strlen(dist) + strlen(source) + 1);
    strcpy(newString,dist);
    strcat(newString,source);
    return newString;
}

