#include "client.h"
int SendInt(int socket, int i)
{
    int intToSend = htonl(i);
    int n = send(socket, &intToSend, sizeof(intToSend),0); // send on Windows
    if (n < 0) {
        perror("ERROR writing to socket from functino [SendInt]\n");
        return -1;
    }
    return 1;
}
