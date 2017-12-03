#include "server.h"
int ReadMessageLength(int socket)
{
    int bytesToReceive = 0;

    int n = recv(socket, &bytesToReceive, sizeof(bytesToReceive),0);
    printf("Waiting message len:[%d]", bytesToReceive);
    if(n < 0)
        printf("Reading ERROR for function ReadMessageLength\n");

    int bytes = ntohl(bytesToReceive);

    return bytes;
}
