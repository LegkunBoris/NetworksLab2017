#include "client.h"
int client(int argc , char *argv[]){

    printf("Client v1.0\n@Author Dango\n");

rec:if((csocket = init(argc,argv)) < 1)
        printf("Init connection error\n");
    else
        printf("Connected\n");
nm: printf("Enter ur name:");
    int n = NameFunction();

    if(n < 0)
    {
        printf("Unregistered user!\n");
        goto nm;
    }

    printf("Enter request (""help"" to get description):\n");
    while(TRUE){
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
            case DISPLAY:
            {
                int n = DisplayFunction(command);
                break;
            }
            case ONLINE:
            {
                int n = OnlineFunction(command);
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
char* GenerateCommand(int com, char* key)
{

    char comToChar[BUFFLEN];

    itoa(com,comToChar,10);

    char *tmp = "";
    char *command = concatStrings(tmp, comToChar);
    command = concatStrings(command, " ");
    command = concatStrings(command, key);
}
int OnlineFunction(int command)
{
    char cmd[BUFFLEN];
    char answer[BUFFLEN];

    itoa(command, cmd,10);

    if(SendBytes(csocket,cmd) < 0)
        printf("Error sending [%d]\n",cmd);

    int numOfUsers = GetReadCount();

    printf("Current online [%d]:\n",numOfUsers);

    for(int i = 0;i < numOfUsers;i++)
    {
        int bytes = ReadBytes(answer);

        answer[bytes] = '\0';

        if(bytes < 0)
            printf("Reading ERROR\n");

        printf("[%s]\n",answer);
    }
    return 1;
}

int NameFunction()
{
    scanf("%s",name);

    int n;

    char *command = GenerateCommand(NAME,name);

    if(n = send(csocket,command, strlen(command),0) < 0)
        printf("Sending error [%s]\n",name);

    char answer[BUFFLEN];

    n = ReadBytes(answer);

    int i = atoi(answer);
    if(i == NAME)
        return 1;
    return -1;
}

int QuitFunction(int command)
{
    char cmd[BUFFLEN];
    char answer[BUFFLEN];

    itoa(command, cmd,10);

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
    char answer[BUFFLEN];

    itoa(command, cmd,10);

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
int DisplayFunction(int command)
{
    char cmd[BUFFLEN];
    char answer[BUFFLEN];

    itoa(command, cmd,10);

    if(SendBytes(csocket,cmd) < 0)
        printf("Error sending [%d]\n",cmd);

    int numOfForums = GetReadCount();

    for(int i = 0;i < numOfForums;i++)
    {
        char forum[BUFFLEN];

        int n = ReadBytes(forum);
        forum[n] = '\0';
        printf("[%s]:\n",forum);
        int numOfMessages = GetReadCount();

        for(int j = 0; j < numOfMessages;j++)
        {
            int bytes = ReadBytes(answer);

            answer[bytes] = '\0';

            if(bytes < 0)
                printf("Reading ERROR\n");

            printf("\t%s\n",answer);
        }
    }
    return 1;
}
int ForumsFunction(int command)
{
    char cmd[BUFFLEN];
    char answer[BUFFLEN];

    itoa(command, cmd,10);

    if(SendBytes(csocket,cmd) < 0)
        printf("Error sending [%d]\n",cmd);

    int numOfForums = GetReadCount();

    printf("Forums[%d]:\n",numOfForums);

    for(int i = 0;i < numOfForums;i++)
    {
        int bytes = ReadBytes(answer);

        answer[bytes] = '\0';

        if(bytes < 0)
            printf("Reading ERROR\n");

        printf("[%s]\n",answer);
    }
    return 1;
}

int OpenFunction(int com, char *token)
{
    char *command = GenerateCommand(com, token);

    if(SendBytes(csocket,command) < 0)
        printf("Sending %d error for OpenFunction\n",command);

    char answer[BUFFLEN];

    int n = ReadBytes(answer);

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
                if(SendBytes(csocket,message) < 0)
                    printf("Sending %d error\n",message);
                break;
            }

            messageToSend = concatStrings(name,":");
            messageToSend = concatStrings(messageToSend,message);

            if(SendBytes(csocket,messageToSend) < 0)
                printf("Sending %d error\n",messageToSend);
        }
    }
    else
    {
        printf("Forum [%s] doesnt exist.\n",token);
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
