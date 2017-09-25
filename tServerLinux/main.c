/* Код сервера на си
Выполнил студент группы 43501/1
Легкун Борис
*/
#include <stdio.h>
#include <stdlib.h>

#include <netdb.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>

#include <string.h>

#include <pthread.h>
void *connection_handler(void *socket_desc);
int main(int argc, char *argv[]) {
    int sockfd, newsockfd;
    uint16_t portno;
    unsigned int clilen;
    struct sockaddr_in serv_addr, cli_addr;

    /* First call to socket() function */
    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if (sockfd < 0) {
        perror("ERROR opening socket");
        exit(1);
    }

    /* Initialize socket structure */
    bzero((char *) &serv_addr, sizeof(serv_addr));
    portno = (uint16_t) atoi(argv[1]);

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);

    /* Now bind the host address using bind() call.*/
    if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
    {
        perror("ERROR on binding");
        exit(1);
    }

    /* Now start listening for the clients, here process will
       * go in sleep mode and will wait for the incoming connection
    */

    printf("Привязка выполнена.\n");

    //Listen
    listen(sockfd , 5);

    //Accept and incoming connection
    puts("Ожидаем подключения...");
    clilen = sizeof(struct sockaddr_in);

    pthread_t thread_id;

    while( (newsockfd = accept(sockfd, (struct sockaddr *)&cli_addr, (socklen_t*)&clilen)) )
    {
        if( pthread_create( &thread_id , NULL ,  connection_handler , (void*) &newsockfd) < 0)
        {
            perror("could not create thread");
            return 1;
        }
        //Now join the thread , so that we dont terminate before the thread
        //pthread_join( thread_id , NULL);
        printf("Поток создан для клиента: %s; Порт: %d\n",inet_ntoa(cli_addr.sin_addr) ,ntohs(cli_addr.sin_port));
    }

    if (newsockfd < 0)
    {
        perror("accept failed");
        return 1;
    }

    shutdown(newsockfd,SHUT_RDWR);
    close(newsockfd);
    shutdown(sockfd,SHUT_RDWR);
    close(sockfd);
    return 0;
}
int readn(int *socket){
    char buffer[256];
    bzero(buffer, 256);
    int n;
    int length = 0;
    int first_msg = 1;
    while( 1 ){
        n = read(socket, buffer, 255);
        if (n < 0) return -1;
        else
        {
            if(first_msg) {printf("Полученное сообщение:");first_msg=0;}
            if(n < 255){length += n;printf("%s\n", buffer);return length;}
            else{length += n; printf("%s", buffer);}
        }
        bzero(buffer, 256);
    }
    return -1;
}
/*
 * This will handle connection for each client
 * */
void *connection_handler(void *socket_desc)
{
    //Get the socket descriptor
    int sock = *(int*)socket_desc;
    /* Read msg */
    int len = readn(sock);
    printf("Длина сообщения:%d\n", len);

    if(len == -1)
    {
        perror("read failed");
    }
    int n = write(sock, "I got your message", 18); // send on Windows
    if (n < 0) {
        perror("ERROR writing to socket");
        exit(1);
    }

    return 0;
}
