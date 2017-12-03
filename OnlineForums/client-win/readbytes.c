#include "client.h"
int ReadBytes(char *answer)
{
    int bytes = GetReadCount();

    int n = recv(csocket,answer,bytes,0);

    if(n < 0)
        printf("Reading ERROR for function ReadBytes\n");

    return bytes;
}
