#include "client.h"
int client(int argc,char *argv[]){

    printf("Client v1.0\n@Author Dango\n");
    RunConfig(argc,argv);
    printf("Enter request (""help"" to get description):\n");
    while(TRUE){
        char buffer[BUFFLEN],*saveptr,*command,*argument;
        int icommand;

        memset(buffer,0,strnlen(buffer,MAXCOUNT));
        printf("%s:",name);
        do{fgets(buffer,BUFFLEN,stdin);
        }while((strnlen(buffer,MAXCOUNT) - 1) == 0);
        buffer[strnlen(buffer,MAXCOUNT) - 1] = '\0';
        if(command = strtok_r(buffer," ",&saveptr))
            icommand = DecodeString(command);
        argument = strtok_r(NULL," ",&saveptr);
        switch(icommand){
            case CREATE:{
                int n = CreateFunction(icommand,argument);
                break;
            }
            case DISPLAY:
            {
                int n = DisplayFunction(icommand);
                break;
            }
            case ONLINE:
            {
                int n = OnlineFunction(icommand);
                break;
            }
            case OPEN:{
                int n = OpenFunction(icommand,argument);
                break;
            }
            case HELP:  {
                int n = HelpFunction(icommand);
                break;
            }
            case FORUMS:  {
                int n = ForumsFunction(icommand);
                break;
            }
            case RECON: {
                RunConfig(argc,argv);
                break;
            }
            case QUIT:{
                int n = QuitFunction(icommand);
                break;
            }
            default:    {
                printf("command %s not found\n",command);
                printf("hash = %d\n",icommand);
                break;
            }
        }
    }
    return 1;
}
void RunConfig(int argc,char *argv[])
{
    int status = RunInit(argc,argv);
    RunLogin(status);
}

int RunLogin(int status)
{
    int n = -1;
    while(n < 0 && status == 1)
    {
        printf("Enter ur name:");
        n = NameFunction();
        if(n < 0)
        {
            printf("Unregistered user!\n");
        }
    }
}

int RunInit(int argc,char *argv[])
{
    if((csocket = Init(argc,argv)) < 1)
    {
        printf("Connection error\n");
        return -1;
    }
    else
        printf("Connected\n");
    return 1;
}

char* GenerateCommand(int com,char* key)
{
    char comToChar[BUFFLEN],*command;

    itoa(com,comToChar,10);
    command = ConcatStrings("",comToChar);
    command = ConcatStrings(command," ");
    command = ConcatStrings(command,key);
    return command;
}
int OnlineFunction(int command)
{
    char answer[BUFFLEN];
    int numOfUsers, bytes;

    numOfUsers = SendRequestWithReadInt(command,"");
    printf("Current online [%d]:\n",numOfUsers);
    for(int i = 0;i < numOfUsers;i++)
    {
        bytes = ReadBytes(answer);
        answer[bytes] = '\0';
        if(bytes < 0)
            printf("Reading ERROR\n");
        printf("[%s]\n",answer);
    }
    return 1;
}

int NameFunction()
{
    char answer[BUFFLEN];
    int n;
    scanf("%s",name);
    n = SendRequestWithReadBytes(NAME,name,answer);
    if(atoi(answer) == NAME)
        return 1;
    return -1;
}

int QuitFunction(int command)
{
    char answer[BUFFLEN];
    int n;
    n = SendRequestWithReadBytes(command,"",answer);
    if(DecodeString(answer) == QUIT)
    {
        printf("You are disconnected!\n");
        closesocket(csocket);
        WSACleanup();
    }
    return 1;
}
int HelpFunction(int command)
{
    char answer[BUFFLEN];
    int n;

    n = SendRequestWithReadBytes(command,"",answer);
    if(n < 0)
    {
        printf("Reading ERROR\n");
    }
    printf("\n*****************************************\n%s*****************************************\n",answer);
    return 1;
}
int DisplayFunction(int command)
{
    char answer[BUFFLEN],forum[BUFFLEN];
    int bytes,n,numOfForums,numOfMessages;

    numOfForums = SendRequestWithReadInt(command,"");
    for(int i = 0;i < numOfForums;i++)
    {
        n = ReadBytes(forum);
        forum[n] = '\0';
        printf("[%s]:\n",forum);
        numOfMessages = ReadInt();
        for(int j = 0; j < numOfMessages;j++)
        {
            bytes = ReadBytes(answer);
            answer[bytes] = '\0';
            if(bytes < 0)
                printf("Reading ERROR\n");
            printf("\t%s\n",answer);
        }
        forum[0] = '\0';
    }
    return 1;
}
int ForumsFunction(int command)
{
    char answer[BUFFLEN];
    int numOfForums,bytes;

    numOfForums = SendRequestWithReadInt(command,"");
    printf("Forums[%d]:\n",numOfForums);
    for(int i = 0;i < numOfForums;i++)
    {
        bytes = ReadBytes(answer);
        answer[bytes] = '\0';
        if(bytes < 0)
            printf("Reading ERROR\n");
        printf("[%s]\n",answer);
        answer[0] = '\0';
    }
    return 1;
}

int OpenFunction(int com,char *argument)
{
    char answer[BUFFLEN];
    int n;

    n = SendRequestWithReadBytes(com,argument,answer);
    if(strcmp(answer,argument) == 0)
    {
        HANDLE thread = CreateThread(NULL,0,ChatHandler,NULL,0,NULL);
        forum = argument;
        while(TRUE)
        {
            char message[BUFFLEN];
            char *messageToSend;

            printf("[%s]%s:",argument,name);
            fgets(message,BUFFLEN,stdin);
            message[strnlen(message,MAXCOUNT) - 1] = '\0';
            if(strcmp(message,"--quit") == 0)
            {
                if(SendBytes(csocket,message) < 0)
                    printf("Sending %d error\n",message);
                break;
            }
            messageToSend = ConcatStrings(name,":");
            messageToSend = ConcatStrings(messageToSend,message);
            if(SendBytes(csocket,messageToSend) < 0)
                printf("Sending %d error\n",messageToSend);
            free(messageToSend);
        }
    }
    else
    {
        printf("Forum [%s] doesnt exist.\n",argument);
        return -1;
    }
    return 1;
}
int CreateFunction(int command,char *argument)
{
    SendRequest(command,argument);
    return 1;
}
void SendRequest(int command,char *argument)
{
    char *cmd = GenerateCommand(command,argument);
    if(SendBytes(csocket,cmd) < 0)
        printf("Sending %d error for OpenFunction\n");
    free(cmd);
}
int SendRequestWithReadBytes(int command,char *argument,char *answer)
{
    SendRequest(command,argument);
    return ReadBytes(answer);
}
int SendRequestWithReadInt(int command,char *argument)
{
    SendRequest(command,argument);
    return ReadInt();
}
