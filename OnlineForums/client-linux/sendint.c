#include "client.h"
int SendInt(int socket, int i)
{
    int intToSend = htonl(i);
    int n = write(socket, &intToSend, sizeof(int32_t));
    if (n < 0) {
        perror("ERROR writing to socket from functino [SendInt]\n");
        return -1;
    }
    return 1;
}
