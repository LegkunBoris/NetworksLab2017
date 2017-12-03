#include "server.h"
DWORD WINAPI connection_handler(void *args)
{
    args_t *arg = (args_t*) args;

    //Get the socket descriptor
    int csocket = arg->socket;
    int user_id = nextUser;

    users[user_id].socket = arg->socket;
    users[user_id].id = nextUser;
    users[user_id].ip = arg->ip;
    users[user_id].port = arg->port;
    users[user_id].forum = "";

    nextUser = nextUser + 1;
    while(TRUE){
        char *buffer;
        int len = 0;
start:  free(buffer);
        len = readn(csocket,&buffer);
        if(len == -1)
        {
           return;
        }

        if(!(strlen(buffer) != 0))
            goto start;
        printf("decoding [%s]\n",buffer);

        struct command com;
        char *token;
        char *rest = buffer;

        if(token = strtok_r(rest, " ", &rest))
            com._command = atoi(token);
        token = strtok_r(rest, " ", &rest);
        //printf("int of command:[%d] string command:[%s] token:[%s]\n", com._command, token);

        switch(com._command){
         case CREATE:{
            int n = CreateFunction(users[user_id].socket, token);
            break;
         }
        case DISPLAY:
        {
            int n = DisplayFunction(users[user_id].socket);
            break;
        }
        case ONLINE:
        {
            int n = OnlineFunction(users[user_id].socket, users[user_id].name);
            break;
        }
         case HELP:  {
            int n = HelpFunction(users[user_id].socket,users[user_id].name);
            break;
         }
         case NAME:{
            int n = NameFunction(users[user_id].socket,&users[user_id].name, token);
            printf("User [%s] loged in.\n",users[user_id].name);
            break;
        }
         case OPEN:{
            com._key = -1;
            for(int i = 0;i < forumsCount;i++)
                if(strcmp(forums[i], token) == 0)
                    com._key = i;

            if(com._key != -1)
            {
                users[user_id].forum = forums[com._key];

                printf("User [%s] opened forum [%s]\n",users[user_id].name,users[user_id].forum);

                int n = SendBytes( users[user_id].socket, users[user_id].forum);

                if (n < 0) {
                    perror("ERROR writing to socket\n");
                    return 0;
                }
                char *file_name = concatStrings(token,".txt");

                char *msg = readfromfile(file_name);

                n = SendBytes(users[user_id].socket, msg);

                if (n < 0) {
                    perror("ERROR writing to socket\n");
                    return 0;
                }
                while(TRUE)
                {
                    char message[BUFFLEN];
                    int n = ReadBytes(users[user_id].socket,message);
                    if(n < 0)
                        break;
                    if(strcmp(message,"--quit") == 0)
                    {
                        users[user_id].forum = "";
                        break;
                    }
                    else
                    {
                        printf("Message [%s] from user:[%s]\n",message,users[user_id].name);

                        char *file_name = concatStrings(forums[com._key],".txt");

                        WriteMessageToFile(file_name,message);

                        for(int i = 0;i < usersOnline;i++)
                            if(strcmp(users[i].forum,token) == 0)
                            {
                                if(users[i].id != user_id)
                                {
                                    int n = send(users[i].socket,message, strlen(message),0);
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
                printf("Forum [%s] doesnt exist.\n",token);

                int n = send(csocket, msg, strlen(msg),0);

                if (n < 0) {
                    perror("ERROR writing to socket\n");
                    return 0;
                }
            }
            break;
        }
         case QUIT:{
            QuitFunction(users[user_id].socket, users[user_id].name, user_id );
            return user_id;
         }
         case FORUMS:{
           int n = ForumsFunction(users[user_id].socket, users[user_id].name);
            break;
         }
         default:    {
            printf("command [%s] not found\n",buffer);
            return 0;
         }
    }
    }
}
int NameFunction(int socket, char **user_name, char *name)
{
    if(CheckConnectedUser(name) > 0)
    {
        printf("[%s] authorized\n", name);
        char *newString = (char *)malloc(strlen(name));
        strcpy(newString,name);
        *user_name = newString;

        char cmd[BUFFLEN];
        itoa(NAME, cmd,10);

        int n = SendBytes(socket, cmd);

        printf("[%s] authorized\n", name);
        return n;
    }
    char cmd[BUFFLEN];
    itoa(UNAME, cmd,10);
    int n = SendBytes(socket, cmd);
    return -1;
}
int CheckConnectedUser(char *name)
{
    FILE *fp;
    if ((fp=fopen("users.txt", "r") ) == NULL) {
        printf("Cannot open file.\n");
        return 1;
    }

    char tmp[256];
    while(fgets(tmp, sizeof tmp, fp)) {
        tmp[strlen(tmp) - 1] = '\0';
        if(strcmp(tmp, name) == 0)
        {
            fclose(fp);
            return 1;
        }
    };

    fclose(fp);
    return -1;
}

int HelpFunction(int csocket, char *name)
{
    char *msg = readfromfile("help.txt");

    int n = send(csocket, msg, strlen(msg),0);

    if (n < 0) {
        perror("ERROR writing to socket\n");
        return -1;
    }
    free(msg);

    printf("Help sent to user:[%s]\n",name);
    return 1;
}

void QuitFunction(int socket, char *name, int user_id)
{
    printf("Closing connection to [%s]\n",name);

    char msg[] = "quit";

    int n = send(socket, msg, strlen(msg),0);

    if (n < 0) {
        perror("ERROR writing to socket\n");
        return 0;
    }
    usersOnline = usersOnline - 1;

    closesocket(socket);

    nextUser = user_id;

    users[user_id] = ClearUser;

    free(msg);

    closesocket(socket);
}

int ForumsFunction(int csocket, char *name)
{
    int intSend = SendInt(csocket, forumsCount);

    for(int i = 0; i < forumsCount;i++)
    {
        printf("Sending forums:[%s]\n",forums[i]);
        intSend = SendBytes(csocket,forums[i]);
    }
    printf("list of forums has been sent to user:[%s]\n", name);
    return 1;
}
int OnlineFunction(int csocket, char *name)
{
    int intSend = SendInt(csocket, nextUser);

    for(int i = 0; i < nextUser;i++)
    {
        printf("Sending forums:[%s]\n",users[i].name);
        int n = SendBytes(csocket,users[i].name);
    }
    printf("list of users online has been sent to user:[%s]\n", name);
    return 1;
}
int CreateFunction(int csocket, char *token )
{
    printf("in create\n");
    for(int i = 0; i < forumsCount;i++)
    {
        if(forums[i] == token){

            char msg[] = "Forums already exists!";

            int n = send(csocket, msg, strlen(msg),0);

            if (n < 0) {
                perror("ERROR writing to socket\n");
                return -1;
            }
        }
    }

    printf("check on exist done\n");

    void *newString = malloc(strlen(token));

    strcpy(newString,token);

    forums[forumsCount++] = newString;

    for(int i = 0; i < forumsCount;i++)
         printf("forum[%d] [%s]\n",i,forums[i]);

    FILE *f = fopen("forums.txt","a");
    if (f == NULL)
    {
        printf("Error opening file!\n");
        return -2;
    }
    fprintf(f, token);
    fprintf(f, "\n");
    fclose(f);

    printf("Forum [%s] added to forum list.\n",token);

    char *file_name = concatStrings(token,".txt");

    f = fopen(file_name ,"w");

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
    int intSend = SendInt(socket, forumsCount);

    int n = 0;
    char *file_name;
    int lines = 0;
    char tmp[BUFFLEN];
    for(int i = 0; i < forumsCount;i++)
    {
        printf("forum prepared to send[%s]\n",forums[i]);
        n = SendBytes(socket,forums[i]);

        file_name = concatStrings(forums[i],".txt");

        lines = readlinesfromfile(file_name);

        intSend = SendInt(socket, lines);

        printf("openning file[%s]\n",file_name);
        FILE *fp;
        if ((fp = fopen(file_name, "r") ) == NULL) {
            printf("Cannot open file.\n");
            return -1;
        }

        printf("Sending context of file[%s] with [%d] lines\n",file_name,lines);

        for(int j = 0; j < lines;j++)
        {
            fgets(tmp, sizeof tmp, fp);
            if(strlen(tmp) > 0)
            {
                tmp[strlen(tmp) - 1] = '\0';
                printf("Sending [%s]\n",tmp);

                n = SendBytes(socket,tmp);
            }
        }
        fclose(fp);
        printf("file closed[%s]\n",file_name);
    }
    return 1;
}
