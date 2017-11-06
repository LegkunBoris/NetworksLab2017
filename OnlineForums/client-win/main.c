//#define _GNU_SOURCE
#include "client.h"
int main(int argc , char *argv[])
{

    if (argc < 3) {
        printf("Params ERROR.\nExample:client-win ""server_ip"" ""server_port""\n");
        exit(0);
    }
    client(argc,argv);
    return 0;
}
