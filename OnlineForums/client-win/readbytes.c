#include "client.h"
int ReadBytes(char *answer)
{
    int bytes = ReadInt();
    int n = recv(csocket,answer,bytes,0);
    if(n < 0)
        printf("Reading ERROR for function ReadBytes\n");
    answer[bytes] = '\0';
    return bytes;
}
