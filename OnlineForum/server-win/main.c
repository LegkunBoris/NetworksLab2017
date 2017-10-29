/*
    Create a TCP socket
*/

#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>
#include <pthread.h>

#define HELP 2090324718
#define NAME 2090536006
#define QUIT 2090665480
#define maxClients 5
#define bufSize 256
struct user{
    int socket;
    char *name;
    char *ip;
    int port;
    int id;

};

typedef struct args_tag {
    int socket;
    char *ip;
    int port;
} args_t;

pthread_t threads[maxClients];
struct user users[maxClients];
int usersOnline = 0;
int nextUser = 0;
args_t args;

static const struct user ClearUser;

void *connection_handler(void *args);
char* concatStrings(char *dist, char *source);
char* GetHelp(const char *path);

const unsigned long hash(const char *str) {
    unsigned long hash = 5381;
    int c;

    while (c = *str++)
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

    return hash;
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
    listen(serverSocket , maxClients);
    printf("Waiting for incoming connections...\n");
    int clilen = sizeof(struct sockaddr_in);
    pthread_t thread_id;

    while( (accSocket = accept(serverSocket, (struct sockaddr *)&client, (socklen_t*)&clilen)) )
    {
        args.ip = inet_ntoa(client.sin_addr);
        args.port = ntohs(client.sin_port);
        args.socket = accSocket;
        if( pthread_create( &thread_id , NULL ,  connection_handler , (void*) &args) < 0)
        {
            perror("could not create thread");
            return 1;
        }
        //Now join the thread , so that we dont terminate before the thread
        //pthread_join( thread_id , NULL);
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
int readn(int *socket,char** output){
    char buffer[256];
    int length = 0;
    while( 1 ){
        memset(buffer,0,strlen(buffer));
        int n = recv(socket, buffer, 255,0);
        if (n < 0) return -1;
        else
        {
            if(n < 255){
                length += n;
                buffer[n] = '\0';
                *output = concatStrings("",buffer);
                printf("output:[%s]",*output);
                return length;
            }
            else{
                length += n;
            }
        }
    }
    return -1;
}
/*
 * This will handle connection for each client
 * */
void *connection_handler(void *args)
{
    args_t *arg = (args_t*) args;

    //Get the socket descriptor
    int csocket = arg->socket;
    int user_id = nextUser;
    users[nextUser].socket = arg->socket;
    users[nextUser].id = nextUser;
    users[nextUser].ip = arg->ip;
    users[nextUser].port = arg->port;

    nextUser = nextUser + 1;
    while(1){
        char *buffer;
        int len = readn(csocket,&buffer);
        if(len == -1)
        {
           return;
        }
        printf("[%s]",buffer);
        switch(hash(buffer)){
         case HELP:  {
            char *msg = GetHelp("help.txt");
            int n = sendmsg(csocket,msg);

            if (n < 0) {
                perror("ERROR writing to socket\n");
                return 0;
            }
            //free(msg);
            break;
         }
        case NAME:{
            char *name;
            len = readn(csocket,&name);
            users[user_id].name = name;
            free(name);
            break;
        }
        case QUIT:{
            printf("Closing connection to [%s]\n",users[user_id].name);
            char msg[] = "quit";
            int n = send(csocket, msg, sizeof msg,0);
            if (n < 0) {
                perror("ERROR writing to socket\n");
                return 0;
            }
            usersOnline = usersOnline - 1;
            closesocket(users[user_id].socket);
            nextUser = user_id;
            users[user_id] = ClearUser;
            free(msg);
            break;
        }
         default:    {
            printf("command [%s] not found\n",buffer);
            printf("hash = %d\n",hash(buffer));
            break;
         }

    }
    free(buffer);
    /*int n = write(sock, "I got your message", 18); // send on Windows
    if (n < 0) {
        perror("ERROR writing to socket");
        exit(1);
    }
    */
    //printf("Closing connection");
    //closesocket(sock);
    }
}
int sendmsg(int *socket,char *msg){
    int n = send(socket, msg, sizeof msg,0); // send on Windows
    if (n < 0) {
        perror("ERROR writing to socket");
        exit(1);
    }
    return n;
}

char* GetHelp(const char *path)
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
    void *newString = malloc(strlen(dist) + strlen(source));
    strcpy(newString,dist);
    strcat(newString,source);
    return newString;
}

