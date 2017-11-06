#include "client.h"
DWORD WINAPI chat_handler(void *args)
{
    while(TRUE)
    {
        char answer[BUFFLEN];
        int n = readn(csocket, answer,0);
        printf("\n%s\n",answer);
    }
}
