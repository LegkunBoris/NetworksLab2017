/* Код клиента на си
Выполнил студент группы 43501/1
Легкун Борис
*/
#include <stdio.h>
#include <stdlib.h>

#include <netdb.h>
#include <netinet/in.h>
#include <unistd.h>

#include <string.h>

int main(int argc, char *argv[]) {
    int sockfd, n;
    uint16_t portno;
    struct sockaddr_in serv_addr;
    struct hostent *server;

    char buffer[256];

    if (argc < 3) {
        fprintf(stderr, "usage %s hostname port\n", argv[0]);
        exit(0);
    }

    portno = (uint16_t) atoi(argv[2]);

    /* Create a socket point */
    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if (sockfd < 0) {
        perror("ERROR opening socket");
        exit(1);
    }

    server = gethostbyname(argv[1]);

    if (server == NULL) {
        fprintf(stderr, "ERROR, no such host\n");
        exit(0);
    }

    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy(server->h_addr, (char *) &serv_addr.sin_addr.s_addr, (size_t) server->h_length);
    serv_addr.sin_port = htons(portno);

    /* Now connect to the server */
    if (connect(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
        perror("ERROR connecting");
        exit(1);
    }

    /* Now ask for a message from the user, this message
       * will be read by server
    */

    printf("Please enter the message: ");
    char* buf = NULL;
    size_t len;
    
    if(getline(&buf, &len, stdin) == -1 && ferror(stdin))
        err(1, "getline");
    printf("Длина сообщения:%d\n", strlen(buf));
    /* Send message to the server */
    n = write(sockfd, buf, strlen(buf));
    if (n < 0) {
        perror("ERROR writing to socket");
        exit(1);
    }
    /* Now read server response */
    int l = readn(sockfd);
    printf("Длина сообщения:%d\n", l);
    /* Close socket */
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
