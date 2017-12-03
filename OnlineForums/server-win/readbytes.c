#include "server.h"
int ReadBytes(int csocket, char *answer)
{
    int bytes = ReadMessageLength(socket);

    int n = recv(csocket,answer,bytes,0);

    if(n < 0)
        printf("Reading ERROR for function ReadBytes\n");

    return bytes;
}
