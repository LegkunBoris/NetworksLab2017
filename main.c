#include <stdio.h>
#include <stdlib.h>

#include <netdb.h>
#include <netinet/in.h>
#include <unistd.h>

#include <string.h>

int main(int argc, char *argv[]) {
    int sockfd, newsockfd;
    uint16_t portno;
    unsigned int clilen;
    char buffer[256];
    struct sockaddr_in serv_addr, cli_addr;
    ssize_t n;

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
    if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
        perror("ERROR on binding");
        exit(1);
    }

    /* Now start listening for the clients, here process will
       * go in sleep mode and will wait for the incoming connection
    */

    listen(sockfd, 5);
    clilen = sizeof(cli_addr);

    /* Accept actual connection from the client */
    newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);

    if (newsockfd < 0) {
        perror("ERROR on accept");
        exit(1);
    }

    /* Read msg */
    readn(newsockfd);

    /* Write a response to the client */
    n = write(newsockfd, "I got your message", 18); // send on Windows
    if (n < 0) {
        perror("ERROR writing to socket");
        exit(1);
    }
    shutdown(newsockfd,SHUT_RDWR);
    close(newsockfd);
    shutdown(sockfd,SHUT_RDWR);
    close(sockfd);
    return 0;
}
void readn(int *socket){
	char buffer[256];
	bzero(buffer, 256);
	int n;
    int first_msg = 1;
	while( 1 ){
		n = read(socket, buffer, 255);
		if (n < 0) {
			perror("ERROR reading from socket");
			exit(1);
		}
		else 
        {   
            if(first_msg) {printf("Полученное сообщение:");first_msg=0;}
            if(n < 255){
                printf("%s\n", buffer);
                return;
            }
            else 
			    printf("%s", buffer);
        }
		bzero(buffer, 256);
	}
	return;
}
