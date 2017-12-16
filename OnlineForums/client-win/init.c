#include "client.h"
int Init(int argc , char *argv[]){
    WSADATA wsa;
    SOCKET csocket;
    struct sockaddr_in server;

    if (WSAStartup(0x0202,&wsa) != 0)
    {
        printf("Failed. Error Code : %d",WSAGetLastError());
        return 0;
    }
    //Create a socket
    if((csocket = socket(AF_INET , SOCK_STREAM , 0 )) == INVALID_SOCKET)
    {
        printf("Could not create socket : %d" , WSAGetLastError());
        return 0;
    }

    server.sin_addr.s_addr = inet_addr(argv[1]);
    server.sin_family = AF_INET;
    server.sin_port = htons( (UINT16)atoi(argv[2]));

    //Connect to remote server
    if (connect(csocket , (struct sockaddr *)&server , sizeof(server)) < 0)
    {
        return 0;
    }
    return csocket;
}
