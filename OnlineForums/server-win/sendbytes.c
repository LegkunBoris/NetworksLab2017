#include "server.h"
int SendBytes(int socket, char *message)
{
    int n = SendInt(socket, strlen(message));
    n = send(socket, message, strlen(message),0); // send on Windows
    if (n < 0) {
        perror("ERROR writing to socket");
        return -1;
    }
    return n;
}
