#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

#include "net.h"
#include "server.h"

void acceptTcpHandler(aeEventLoop *el, int fd, void *privdata, int mask);
void readQueryFromClient(aeEventLoop *el, int fd, void *clientData, int mask);

void acceptTcpHandler(aeEventLoop *el, int fd, void *clientData, int mask){
    struct sockaddr_in cliaddr;
    socklen_t cliaddrlen;
    int clifd = accept(fd, (struct sockaddr *)NULL, NULL);
    if(clifd > 0){
        Client *client= (Client *)malloc(sizeof(struct Client));
        client->clientfd = clifd;
        client->readProc = readQueryFromClient;
        anetSetBlock(clifd, 1);
        aeCreateFileEvent(el, clifd, AE_READABLE, client->readProc, client);
    }else{
        perror("fail to accept client socket.");
    }
}

void readQueryFromClient(aeEventLoop *el, int fd, void *clientData, int mask){
    Client *client = clientData;
    int clientfd = client->clientfd;
    char *buf = client->buf;
    ssize_t nread = read(fd, buf + client->writeIndex, CONN_BUF_SIZE);
    if (nread <= 0) {
        if (nread == 0){
             fprintf(stderr, "client close.\n");
        }else{
            perror("read error");
            close(fd);
        }
    } else {
        client->writeIndex += nread;
        if(client->writeIndex >= CONN_BUF_SIZE){
            client->writeIndex = 0;
        }
        printf("read msg is: %s, write index is:%d\n", buf,  client->writeIndex);
    }

}

int main(int argc, char* argv[]){
    printf("net demo start...pid is: %d\n", getpid());

    Server *server = (Server *)malloc(sizeof(*server));
    int listenfd = socket_bind(IPADDRESS, PORT);
    server->listenfd = listenfd;
    listen(listenfd, LISTENQ);
    server->ev = aeCreateEventLoop(1024);
    aeCreateFileEvent(server->ev, listenfd, AE_READABLE, acceptTcpHandler, NULL);

    while(1){
        aeMain(server->ev);
    }
    aeStop(server->ev);

    // int pid_arr[WORKER_SIZE];
    // for(int i = 0; i< WORKER_SIZE; i++){
    //     int cid = 0;
    //     if((cid = fork()) == 0){
    //         printf("in child process...pid is %d, ppid is %d, the listenfd is %d\n", getpid(), getppid(), listenfd);
    //         listen(listenfd, LISTENQ);
    //         do_epoll(listenfd);
    //     }else if(cid < 0){
    //         printf("fork error...\n");
    //         return -1;
    //     }

    //     pid_arr[i] = cid;
    // }

    // int status;
    // waitpid(-1, &status, WNOHANG);
    // printf("exit the  parent process...\n");
    return 0;
}