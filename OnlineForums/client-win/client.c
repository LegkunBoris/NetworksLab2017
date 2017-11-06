#include "client.h"
int client(int argc , char *argv[]){
    printf("Client v1.0\n@Author Dango\n");
rec:if((csocket = init(argc,argv)) < 1)
        printf("Init connection error\n");
    else
        printf("Connected\n");
    char name[80];
    printf("Enter ur name:");
    scanf("%s",name);
    int n;
    if( n = send(csocket,"name\n", strlen("name"),0) < 0)
        printf("Sending [name] error\n");
    if(n = send(csocket,name, strlen(name),0) < 0)
        printf("Sending error [%s]\n",name);


    printf("Enter request (""help"" to get description):\n");
    while(TRUE){
        char buffer[BUFFLEN];
        memset(buffer,0,strlen(buffer));
        printf("%s:",name);
        fgets(buffer,BUFFLEN,stdin);
        buffer[strlen(buffer) - 1] = '\0';
        struct command com;
        char *token;
        char *rest = buffer;

        if(token = strtok_r(rest, " ", &rest))
            com._command = decodeString(token);
        if(token = strtok_r(rest, " ", &rest))
            com._key = decodeString(token);

        switch(com._command){
            case CREATE:{
                char *msg = concatStrings(buffer," ");
                msg = concatStrings(msg, token);
                printf("[%s] has been sent\n",msg);
                if(send(csocket,msg, strlen(msg),0) < 0)
                    printf("Sending error [%s]\n",buffer);
                break;
            }
            case OPEN:{
                char *msg = concatStrings(buffer," ");
                msg = concatStrings(msg, token);
                if(send(csocket,msg, strlen(msg),0) < 0)
                    printf("Sending %d error\n",msg);
                char answer[BUFFLEN];
                printf("reading\n");
                int n = readn(csocket, answer,0);
                printf("got [%d] bytes\n",n);
                if(strcmp(answer, token) == 0)
                {
                    HANDLE thread = CreateThread(NULL,0,chat_handler,NULL,0,NULL);
                    while(TRUE)
                    {
                        char message[BUFFLEN];
                        char *messageToSend;
                        memset(message,0,strlen(message));
                        printf("[%s]%s:",token,name);
                        fgets(message,BUFFLEN,stdin);
                        message[strlen(message) - 1] = '\0';
                        if(strcmp(message,"--quit") == 0)
                        {
                            if(send(csocket,message, strlen(message),0) < 0)
                                printf("Sending %d error\n",message);
                            break;
                        }
                        messageToSend = concatStrings(name,":");
                        messageToSend = concatStrings(messageToSend,message);
                        if(send(csocket,messageToSend, strlen(messageToSend),0) < 0)
                            printf("Sending %d error\n",messageToSend);


                    }
                }
                else
                {
                    printf("Forums [%s] doesnt exist.\n",token);
                    break;
                }
                break;
            }
            case HELP:  {
                char msg[] = "help";
                char answer[BUFFLEN];
                printf("sending help\n");
                if(send(csocket,msg, strlen(msg),0) < 0)
                    printf("Error sending [%s]\n",msg);
                int l = readn(csocket,answer,1);
                if(l < 0)
                    printf("Reading ERROR\n");
                break;
            }
            case FORUMS:  {
                char msg[] = "forums";
                char answer[BUFFLEN];
                if(send(csocket,msg, strlen(msg),0) < 0)
                    printf("Error sending [%s]\n",msg);
                int receivedInt = 0;
                int n = recv(csocket, &receivedInt, sizeof(receivedInt),0);
                int numOfForums = ntohl(receivedInt);
                printf("Forums[%d]:\n",numOfForums);
                for(int i = 0;i < numOfForums;i++)
                {
                    int bytesToReceive = 0;
                    n = recv(csocket, &bytesToReceive, sizeof(bytesToReceive),0);
                    int bytes = ntohl(bytesToReceive);
                    n = recv(csocket,answer,bytes,0);
                    answer[bytes] = '\0';
                    if(n < 0)
                        printf("Reading ERROR\n");
                    printf("[%s]\n",answer);
                }
                free(answer);
                free(msg);
                break;
            }
            case RECON: {
                goto rec;
                break;
            }
            case QUIT:{
                char answer[BUFFLEN];
                if(send(csocket,"quit", strlen("quit"),0) < 0)
                    printf("Send [quit] command error\n");
                int l = readn(csocket,answer,0);
                if(decodeString(answer) == QUIT)
                {
                    printf("You are disconnected!\n");
                    closesocket(csocket);
                    WSACleanup();
                }
                break;
            }
            default:    {
                printf("command %s not found\n",buffer);
                printf("hash = %d\n",com._command);
                break;
            }
            free(buffer);
        }
    }
    return 1;
}
