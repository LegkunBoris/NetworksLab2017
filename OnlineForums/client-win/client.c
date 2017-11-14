#include "client.h"
int client(int argc , char *argv[]){
    printf("Client v1.0\n@Author Dango\n");
rec:if((csocket = init(argc,argv)) < 1)
        printf("Init connection error\n");
    else
        printf("Connected\n");
    printf("Enter ur name:");
    scanf("%s",name);
    char nameToChar[BUFFLEN];
    itoa(NAME,nameToChar,10);
    int n;
    if(n = send(csocket,nameToChar, strlen(nameToChar),0) < 0)
        printf("Sending [name] error\n");
    if(n = send(csocket,name, strlen(name),0) < 0)
        printf("Sending error [%s]\n",name);


    printf("Enter request (""help"" to get description):\n");
    while(TRUE){
        printf("in while\n");
        char buffer[BUFFLEN];
        memset(buffer,0,strlen(buffer));
        printf("%s:",name);
readFromConsole:fgets(buffer,BUFFLEN,stdin);
        if((strlen(buffer) - 1) == 0)
            goto readFromConsole;
        buffer[strlen(buffer) - 1] = '\0';
        int command;
        char *token;
        char *rest = buffer;

        if(token = strtok_r(rest, " ", &rest))
            command = decodeString(token);
        token = strtok_r(rest, " ", &rest);

        switch(command){
            case CREATE:{
                int n = CreateFunction(command,token);
                break;
            }
            case OPEN:{
                int n = OpenFunction(command, token);
                break;
            }
            case HELP:  {
                int n = HelpFunction(command);
                break;
            }
            case FORUMS:  {
                int n = ForumsFunction(command);
                break;
            }
            case RECON: {
                goto rec;
                break;
            }
            case QUIT:{
                int n = QuitFunction(command);
                break;
            }
            default:    {
                printf("command %s not found\n",buffer);
                printf("hash = %d\n",command);
                break;
            }
        }
    }
    return 1;
}

int QuitFunction(int command)
{
    char cmd[BUFFLEN];
    itoa(command, cmd,10);
    char answer[BUFFLEN];
    if(send(csocket,cmd, strlen(cmd),0) < 0)
        printf("Send [quit] command error\n");
    int l = readn(csocket,answer,0);
    if(decodeString(answer) == QUIT)
    {
        printf("You are disconnected!\n");
        closesocket(csocket);
        WSACleanup();
    }
    return 1;
}
int HelpFunction(int command)
{
    char cmd[BUFFLEN];
    itoa(command, cmd,10);
    char answer[BUFFLEN];
    printf("sending help\n");
    if(send(csocket,cmd, strlen(cmd),0) < 0)
        printf("Error sending [%d]\n",cmd);
    int l = readn(csocket,answer,1);
    if(l < 0)
    {
        printf("Reading ERROR\n");
    }
    else
        return -1;
    return 1;
}
int ForumsFunction(int command)
{
    char cmd[BUFFLEN];
    itoa(command, cmd,10);
    char answer[BUFFLEN];
    if(send(csocket,cmd, strlen(cmd),0) < 0)
        printf("Error sending [%d]\n",cmd);
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
    return 1;
}
int OpenFunction(int command, char *token)
{
    char cmd[BUFFLEN];
    itoa(command, cmd,10);
    char *msg = concatStrings(cmd," ");
    msg = concatStrings(msg, token);
    if(send(csocket,msg, strlen(msg),0) < 0)
        printf("Sending %d error\n",msg);
    char answer[BUFFLEN];
    int n = readn(csocket, answer,0);
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
        return -1;
    }
    return 1;
}
int CreateFunction(int command, char *token)
{
    char cmd[BUFFLEN];
    itoa(command, cmd,10);
    char *msg = concatStrings(cmd," ");
    msg = concatStrings(msg, token);
    printf("[%s] has been sent\n",msg);
    if(send(csocket,msg, strlen(msg),0) < 0)
        printf("Sending error [%s]\n",msg);
    return 1;
}
