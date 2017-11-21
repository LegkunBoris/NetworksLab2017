#include "client.h"
DWORD WINAPI chat_handler(void *args)
{
    while(TRUE)
    {
        char answer[BUFFLEN];
        char buffer[BUFFLEN];

        int n = readn(csocket, answer,0);

        //printf("\n%s\n%s:%s",answer, name, line);
    }
}
