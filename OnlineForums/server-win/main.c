
#include "server.h"

int main(int argc , char *argv[])
{
    WSADATA wsa;
    SOCKET serverSocket,accSocket;
    struct sockaddr_in server,client;

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

    server.sin_addr.s_addr  = INADDR_ANY;
    server.sin_family       = AF_INET;
    server.sin_port         = htons( (UINT16)atoi(argv[1]) );

    //Bind
    if( bind(serverSocket ,(struct sockaddr *)&server , sizeof(server)) == SOCKET_ERROR)
    {
        printf("Bind failed with error code : %d" , WSAGetLastError());
    }
    //Listen to incoming connections
    listen(serverSocket , MAXCLIENTS);
    printf("Waiting for incoming connections...\n");

    int clilen          = sizeof(struct sockaddr_in);
    forumsCount         = 0;
    usersOnline         = 0;
    nextUser            = 0;
    forumsCount         = loadForums();

    printf("forums count:[%d]\n", forumsCount);

    while( (accSocket = accept(serverSocket, (struct sockaddr *)&client, (socklen_t*)&clilen)) )
    {

        args.ip         = inet_ntoa(client.sin_addr);
        args.port       = ntohs(client.sin_port);
        args.socket     = accSocket;
        HANDLE thread   = CreateThread(NULL,0,connection_handler,&args,0,NULL);
        usersOnline     = usersOnline + 1;

        printf("Thread created for: %s; on Port: %d\n",inet_ntoa(client.sin_addr) ,ntohs(client.sin_port));
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

