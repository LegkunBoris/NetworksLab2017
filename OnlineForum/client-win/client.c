#include "client.h"
int client(int argc , char *argv[]){
    printf("Client v1.0\n@Author Dango\n");
    int csocket;
rec:if((csocket = init(argc,argv)) < 1)
        printf("Init connection error\n");
    else
        printf("Connected\n");
    char name[80];
    printf("Enter ur name:");
    scanf("%s",name);
    if(sendmsg(csocket,"name") < 0)
        printf("Send [name] command error\n");
    if(sendmsg(csocket,name) < 0)
        printf("Send [%s] error\n",name);


    printf("Enter request (""help"" to get description):\n");
    while(true){
        char buffer[5];
        memset(buffer,0,strlen(buffer));
        printf("user:");
        scanf("%s",buffer);
        switch(hash(buffer)){
            case HELP:  {
                char msg[] = "help";
                char answer[20];
                if(sendmsg(csocket,msg) > 0)
                    printf("Sent %d\n",strlen(msg));
                int l = readn(csocket,answer,1);
                if(l < 0)
                    printf("Reading ERROR\n");
                break;
            }
            case RECON: {
                goto rec;
                break;
            }
            case QUIT:{
                char answer[20];
                if(sendmsg(csocket,"quit") < 0)
                    printf("Send [quit] command error\n");
                int l = readn(csocket,answer,0);
                if(hash(answer) == QUIT)
                {
                    printf("You are disconnected!\n");
                    closesocket(csocket);
                    WSACleanup();
                }
                break;
            }
            default:    {
                printf("command %s not found\n",buffer);
                printf("hash = %d\n",hash(buffer));
                break;
            }
            memset(buffer,0,strlen(buffer));
        }
    }
    return 1;
}
