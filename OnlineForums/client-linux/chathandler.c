#include "client.h"
void ChatHandler()
{
    while(TRUE)
    {
        char answer[BUFFLEN];
        ReadBytes(answer);
        printf("\n%s\n[%s]%s:",answer, forum, name);
    }
}
