#include "client.h"
/* Send message to the server */
int sendmsg(int *csocket,char *msg){
    int n = send(csocket, msg, strlen(msg),0);
    if (n < 0) {
        perror("ERROR writing to socket");
        return 0;
    }
    return n;
}
