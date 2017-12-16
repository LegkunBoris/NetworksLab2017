#include "client.h"
DWORD WINAPI ChatHandler(void *args)
{
    while(TRUE)
    {
        char answer[BUFFLEN];
        ReadBytes(answer);
        printf("\n%s\n[%s]%s:",answer, forum, name);
    }
}
