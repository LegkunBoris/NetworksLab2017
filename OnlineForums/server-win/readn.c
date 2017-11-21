#include "server.h"
//Функция чтения
int readn(int socket,char** output){
    char buffer[BUFFLEN];
    int length = 0;
    char *tmp = "";
    int n;
    while(TRUE)
    {
        n = recv(socket, buffer, BUFFLEN,0);
        length += n;
        tmp = concatStrings(tmp,buffer);
        if(n < BUFFLEN)
            break;
    }
    tmp[length] = '\0';
    *output = tmp;
    return length;
}
