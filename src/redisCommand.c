//
// Created by james on 24-11-14.
//

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <errno.h>
#include <pthread.h>

#include "redisCommand.h"
#include "net.h"

void acceptTcpHandler(aeEventLoop *el, int fd, void *privdata, int mask);
void readQueryFromClient(aeEventLoop *el, int fd, void *clientData, int mask);

void acceptTcpHandler(aeEventLoop *el, int fd, void *clientData, int mask){
    socklen_t cliaddrlen;
    int clifd = accept(fd, (struct sockaddr *)NULL, NULL);
    if(clifd > 0){
        // Client *client= (Client *)malloc(sizeof(struct Client));
        redisClient *client= (redisClient *)malloc(sizeof(struct redisClient));
        client->clientfd = clifd;
        client->readProc = readQueryFromClient;
        anetSetBlock(clifd, 1);
        aeCreateFileEvent(el, clifd, AE_READABLE, client->readProc, client);
    }else{
        perror("fail to accept client socket.");
    }
}

void readQueryFromClient(aeEventLoop *el, int fd, void *clientData, int mask){
    redisClient *client = clientData;
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


static pthread_mutex_t mutex;


void replyData(redisClient *c, void *data, int len) {
    ssize_t send = 0;
    while(send < len) {
        int n = write(c->fd,data + send,len -send);
        if(n == 0) {
           printf("send buff overflow\n");
            continue;
        }

        if(n == -1) {
            if(errno == EAGAIN) {
                printf("EAGAIN\n");
            }
            if(errno == EWOULDBLOCK) {
                printf("EWOULDBLOCK\n");
            }
            printf("send return -1,fail :%d, errno: %m\n", c->fd, errno);
            break;
        }
        send += n;
    }
}

void sendERR(redisClient *c, char* errorInfo) {
    replyData(c,"-", 1);
    replyData(c,errorInfo,sizeof(errorInfo));
    replyData(c,"\r\n",2);
}

void sendStr(redisClient *c, char* data) {
    replyData(c,"+", 1);
    replyData(c,data,sizeof(data));
    replyData(c,"\r\n",2);
}

void sendNil(redisClient *c) {
    replyData(c,"$", 1);
    replyData(c,"-1",2);
    replyData(c,"\r\n",2);
}

void sendBulkStr(redisClient *c, char* data) {
    int len = strlen(data);
    char lenData[32];
    bzero(lenData,sizeof(lenData));
    sprintf(lenData,"%d",len);
    char * cc = strchr(lenData,'\0');
    int c_len = cc - lenData;

    replyData(c,"$", 1);
    replyData(c,lenData,c_len);
    replyData(c,"\r\n",2);
    replyData(c,data,sizeof(data));
    replyData(c,"\r\n",2);
}

void getCommand(redisClient *c, void ** argv) {
    int args = c->argc;
    if(args < 2) {
        printf("wrong number of arguments\n");
        sendERR(c,"ERR");
    }
    dict *db = c->db;
    char *key = argv[1];

    dictEntry *entry = dictFind(db,key);

    // sendBulkStr(c, "ddaa");
    if(entry == NULL) {
        sendNil(c);
        return;
    }
    void *data = entry->v.val;
    sendBulkStr(c, data);
}

void setCommand(redisClient *c, void ** argv) {
    int args = c->argc;
    if(args <= 2) {
        printf("wrong number of arguments\n");
        sendERR(c,"ERR");
    }
    dict *db = c->db;
    char *key = argv[1];
    char *value = argv[2];
    dictAdd(db, key, value);

    // pthread_mutex_init(&mutex,NULL);
    // pthread_mutex_lock(&mutex);
    // dictAdd(db, key, value);
    // pthread_mutex_unlock(&mutex);


    sendStr(c,"OK");
}

void expireCommand(redisClient *c, void ** argv) {

}

void delCommand(redisClient *c, void ** argv) {

}

void infoCommand(redisClient *c, void ** argv) {

}
