#include "server.h"
int ReadMessageLength(int socket)
{
    int bytesToReceive = 0;
    int n = recv(socket, &bytesToReceive, sizeof(int32_t),0);
    if(n < 0)
        printf("Reading ERROR for function ReadMessageLength\n");
    return ntohl(bytesToReceive);
}
