#include "client.h"
int GetReadCount()
{
    int bytesToReceive = 0;

    int n = recv(csocket, &bytesToReceive, sizeof(bytesToReceive),0);

    if(n < 0)
        printf("Reading ERROR for function GetReadCount\n");

    int bytes = ntohl(bytesToReceive);

    return bytes;
}
