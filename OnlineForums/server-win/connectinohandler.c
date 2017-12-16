#include "server.h"
DWORD WINAPI ConnectionHandler(void *args)
{
    argsT *arg = (argsT*) args;

    //Get the socket descriptor
    int userId = nextUser;

    users[userId].socket = arg->socket;
    users[userId].id = nextUser;
    users[userId].ip = arg->ip;
    users[userId].port = arg->port;
    users[userId].forum = "";

    nextUser = nextUser + 1;
    while(TRUE){
        char buffer[BUFFLEN];
        int len = 0;
start:  len = ReadBytes(users[userId].socket,buffer);
        if(len == -1)
        {
           return;
        }
        if(!(strnlen(buffer,MAXCOUNT) != 0))
            goto start;
        printf("decoding [%s]\n",buffer);
        char *command,*argument,*saveptr;
        int icommand;

        if(command = strtok_r(buffer," ",&saveptr))
            icommand = atoi(command);
        argument = strtok_r(NULL," ",&saveptr);
        switch(icommand){
         case CREATE:{
            int n = CreateFunction(users[userId].socket,argument);
            break;
         }
        case DISPLAY:
        {
            int n = DisplayFunction(users[userId].socket);
            break;
        }
        case ONLINE:
        {
            int n = OnlineFunction(users[userId].socket,users[userId].name);
            break;
        }
         case HELP:  {
            int n = HelpFunction(users[userId].socket,users[userId].name);
            break;
         }
         case NAME:{
            int n = NameFunction(users[userId].socket,&users[userId].name,argument);
            printf("User [%s] loged in.\n",users[userId].name);
            break;
        }
         case OPEN:{
            int n = OpenFunction(userId,argument);
            break;
        }
         case QUIT:{
            QuitFunction(users[userId].socket,users[userId].name,userId );
            return userId;
         }
         case FORUMS:{
           int n = ForumsFunction(users[userId].socket,users[userId].name);
            break;
         }
         default:    {
            printf("command [%s] not found\n",buffer);
            return 0;
         }
    }
    }
}
int NameFunction(int socket,char **userName,char *name)
{
    if(CheckConnectedUser(name) > 0)
    {
        printf("[%s] authorized\n",name);
        char *newString = (char *)malloc(strnlen(name,MAXCOUNT));
        strcpy(newString,name);
        *userName = newString;

        char cmd[BUFFLEN];
        itoa(NAME,cmd,10);
        int n = SendBytes(socket,cmd);
        printf("[%s] authorized,socket:[%d]\n",name,socket);
        return n;
    }
    char cmd[BUFFLEN];
    itoa(UNAME,cmd,10);
    int n = SendBytes(socket,cmd);
    return -1;
}
int LoadRegisteredUsers()
{
    int count = 0;
    FILE *fp;
    if ((fp=fopen("users.txt","r") ) == NULL) {
        printf("Cannot open file.\n");
        return 1;
    }

    char tmp[BUFFLEN];
    while(fgets(tmp,sizeof tmp,fp)) {
        char *newString = (char *)malloc(strnlen(tmp,MAXCOUNT));
        strcpy(newString,tmp);
        newString[strnlen(newString,MAXCOUNT) - 1] = '\0';
        reged[count++].Name = newString;
        printf("User [%d] name[%s]\n",count - 1,reged[count - 1].Name);
    };

    fclose(fp);
    return count;
}
int CheckConnectedUser(char *name)
{
    for(int i = 0; i < registeredUsers;i++)
        if(strcmp(reged[i].Name,name) == 0)
            return 1;
    return -1;
}

int HelpFunction(int socket,char *name)
{
    char *msg = ReadFromFile("help.txt");
    int n = SendBytes(socket,msg);
    if (n < 0) {
        perror("ERROR writing to socket\n");
        return -1;
    }
    free(msg);
    printf("Help sent to user:[%s]\n",name);
    return 1;
}

void QuitFunction(int socket,char *name,int userId)
{
    printf("Closing connection to [%s]\n",name);
    char msg[] = "quit";
    int n = SendBytes(socket,msg);
    if (n < 0) {
        perror("ERROR writing to socket\n");
        return 0;
    }
    usersOnline = usersOnline - 1;
    closesocket(socket);
    nextUser = userId;
    users[userId] = ClearUser;
    free(msg);
    closesocket(socket);
}

int ForumsFunction(int socket,char *name)
{
    int intSend = SendInt(socket,forumsCount);
    for(int i = 0; i < forumsCount;i++)
    {
        printf("Sending forums:[%s]\n",forums[i]);
        intSend = SendBytes(socket,forums[i]);
    }
    printf("list of forums has been sent to user:[%s]\n",name);
    return 1;
}
int OnlineFunction(int socket,char *name)
{
    int intSend = SendInt(socket,nextUser);
    for(int i = 0; i < nextUser;i++)
    {
        printf("Sending forums:[%s]\n",users[i].name);
        int n = SendBytes(socket,users[i].name);
    }
    printf("list of users online has been sent to user:[%s]\n",name);
    return 1;
}
int CreateFunction(int socket,char *argument )
{
    char msg[] = "Forums already exists!";
    char *fileName;
    void *newString;
    int n;

    printf("in create\n");
    for(int i = 0; i < forumsCount;i++)
    {
        if(forums[i] == argument){
            n = SendBytes(socket,msg);
            if (n < 0) {
                perror("ERROR writing to socket\n");
                return -1;
            }
        }
    }
    newString = malloc(strnlen(argument,MAXCOUNT));
    strcpy(newString,argument);
    forums[forumsCount++] = newString;
    for(int i = 0; i < forumsCount;i++)
         printf("forum[%d] [%s]\n",i,forums[i]);

    FILE *f = fopen("forums.txt","a");
    if (f == NULL)
    {
        printf("Error opening file!\n");
        return -2;
    }
    fprintf(f,argument);
    fprintf(f,"\n");
    fclose(f);

    printf("Forum [%s] added to forum list.\n",argument);
    fileName = ConcatStrings(argument,".txt");
    f = fopen(fileName ,"w");
    free(fileName);
    if (f == NULL)
    {
        printf("Error opening file!\n");
        return -3;
    }
    fclose(f);
    printf("Forum file added.\n");
    return 1;
}
int DisplayFunction(int socket)
{
    char *fileName,tmp[BUFFLEN];
    int n,lines,intSend;

    intSend = SendInt(socket,forumsCount);
    for(int i = 0; i < forumsCount;i++)
    {
        n = SendBytes(socket,forums[i]);
        fileName = ConcatStrings(forums[i],".txt");
        lines = ReadLinesFromFile(fileName);
        free(fileName);
        intSend = SendInt(socket,lines);
        FILE *fp;
        if ((fp = fopen(fileName,"r") ) == NULL) {
            printf("Cannot open file.\n");
            return -1;
        }
        printf("Sending context of file[%s] with [%d] lines\n",fileName,lines);
        for(int j = 0; j < lines;j++)
        {
            fgets(tmp,sizeof tmp,fp);
            if(strnlen(tmp,MAXCOUNT) > 0)
            {
                tmp[strnlen(tmp,MAXCOUNT) - 1] = '\0';
                printf("Sending [%s]\n",tmp);
                n = SendBytes(socket,tmp);
            }
        }
        fclose(fp);
        printf("file closed[%s]\n",fileName);
    }
    return 1;
}
int OpenFunction(int userId,char *argument)
{
    char *fileName,message[BUFFLEN],*msg;
    int n;
    int forum = -1;

    for(int i = 0;i < forumsCount;i++)
        if(strcmp(forums[i],argument) == 0)
            forum = i;

    if(forum != -1)
    {
        users[userId].forum = forums[forum];
        printf("User [%s] opened forum [%s]\n",users[userId].name,users[userId].forum);
        n = SendBytes( users[userId].socket,users[userId].forum);
        if (n < 0) {
            perror("ERROR writing to socket\n");
            return 0;
        }
        fileName = ConcatStrings(argument,".txt");
        msg = ReadFromFile(fileName);
        n = SendBytes(users[userId].socket,msg);
        free(msg);
        if (n < 0) {
            perror("ERROR writing to socket\n");
            return 0;
        }
        while(TRUE)
        {
            n = ReadBytes(users[userId].socket,message);
            if(n < 0)
                break;
            if(strcmp(message,"--quit") == 0)
            {
                users[userId].forum = "";
                free(fileName);
                break;
            }
            else
            {
                printf("Message [%s] from user:[%s]\n",message,users[userId].name);
                fileName = ConcatStrings(forums[forum],".txt");
                WriteMessageToFile(fileName,message);
                free(fileName);
                for(int i = 0;i < usersOnline;i++)
                    if(strcmp(users[i].forum,argument) == 0)
                    {
                        if(users[i].id != userId)
                        {
                            n = SendBytes(users[i].socket,message);
                            if (n < 0) {
                                perror("ERROR writing to socket\n");
                                return 0;
                            }
                        }
                    }
            }
        }
    }
    else
    {
        char *msg = "error";
        printf("Forum [%s] doesnt exist.\n",argument);
        int n = SendBytes(users[userId].socket,msg);
        if (n < 0) {
            perror("ERROR writing to socket\n");
            return 0;
        }
    }
}
