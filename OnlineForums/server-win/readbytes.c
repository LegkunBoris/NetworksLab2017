#include "server.h"
int ReadBytes(int socket, char *answer)
{
    int bytes = ReadMessageLength(socket);
    int n = recv(socket,answer,bytes,0);
    if(n < 0)
        printf("Reading ERROR for function ReadBytes\n");
    answer[bytes] = '\0';
    return bytes;
}
