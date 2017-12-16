#include "client.h"
int SendBytes(int socket, char *message)
{
    int n = SendInt(socket, strlen(message));
    printf("sendinbg message:[%s]\n",message);
    n = write(socket, message, strlen(message));
    if (n < 0) {
        perror("ERROR writing to socket");
        return -1;
    }
    return n;
}
