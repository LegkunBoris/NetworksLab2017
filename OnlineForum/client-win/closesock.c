#include "client.h"
int disconnect(int csocket){
    closesocket(csocket);
    WSACleanup();
    return 1;
}
