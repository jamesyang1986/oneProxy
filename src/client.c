#include <errno.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>

#define SERVPORT 9527
#define MAXDATASIZE 100 /* max transportation data */

int main(int argc, char **argv) {
    int sockfd, recvbytes;
    char buf[MAXDATASIZE];
    struct hostent *host;
    struct sockaddr_in serv_addr;
    if (argc < 2) {
        fprintf(stderr, "Please enter the server's hostname!\n");
        exit(1);
    }
    if ((host = gethostbyname(argv[1])) == NULL) {
        herror("gethostbyname error! ");
        exit(1);
    }
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("socket create error! ");
        exit(1);
    }
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(SERVPORT);
    serv_addr.sin_addr = *((struct in_addr *)host->h_addr);
    bzero(&(serv_addr.sin_zero), 8);
    if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(struct sockaddr)) == -1) {
        perror("connect error! ");
        exit(1);
    }

    for(int i = 0; i< 500000; i++){
        char *cc = "------asdfadsfadsf-----";
        int size = send(sockfd, cc, strlen(cc), 0);
        if(size < 0){
            perror("send error");
            break;
        }
    }

    if ((recvbytes = recv(sockfd, buf, MAXDATASIZE, 0)) == -1) {
        perror("recv error！");
        exit(1);
    }
    
    buf[recvbytes] = '\0';
    printf("Receive： %s", buf);
//    close(sockfd);
    return (EXIT_SUCCESS);
}
