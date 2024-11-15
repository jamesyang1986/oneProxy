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
#include "server.h"

void acceptTcpHandler(aeEventLoop *el, int fd, void *privdata, int mask);
void readQueryFromClient(aeEventLoop *el, int fd, void *clientData, int mask);
void readClientProc(redisClient *c);


extern Server *server;

redisClient* makeConn(int fd){
    redisClient *client = (redisClient*)malloc(sizeof(redisClient));
    client->fd = fd;
    client->queryBuf = (char*)malloc(sizeof(char)*QUERY_BUFF_SIZE);
    client->sendBuf = (char*)malloc(sizeof(char)*SEND_BUFF_SIZE);
    client->readProc = readQueryFromClient;
    client->connectNum++;
    client->curDb = server->db;
    //set socket not block
    anetSetBlock(fd,1);
    return client;
}


 void acceptTcpHandler(aeEventLoop *el, int fd, void *clientData, int mask){
    socklen_t cliaddrlen;
    int clifd = accept(fd, (struct sockaddr *)NULL, NULL);
    if(clifd > 0){
        redisClient *client= makeConn(clifd);
        aeCreateFileEvent(el, clifd, AE_READABLE, client->readProc, client);
    }else{
        perror("fail to accept client socket.");
    }
}

void readQueryFromClient(aeEventLoop *el, int fd, void *clientData, int mask){
    redisClient *client = clientData;
    readClientProc(client);
}

void readClientProc(redisClient *c){
    ssize_t nread= 0;
    nread = read(c->fd, c->queryBuf + c->writeIndex, QUERY_BUFF_SIZE);
    if(nread == 0) {
        printf("Client read zero\n");
        return;
    }

    if(nread == -1) {
        printf("socket read fail\n");
        return ;
    }

    c->writeIndex += nread;

    int len = (c->writeIndex -c->readIndex);
    char *buff = malloc(sizeof(char) * len);
    memcpy(buff, c->queryBuf + c->readIndex, len);

    printf("From client data: %s\n", buff);

    if(c->reqType == 0) {
        if(buff[0] == '*'){
            c->reqType = PRO_MULTIBULK;
        }else{
            c->reqType = PRO_INLINE;
        }
    }

    if(c->reqType == PRO_INLINE) {
        processInline(c, buff);
    }else if(c->reqType == PRO_MULTIBULK) {
        processMultiBulk(c, buff);
    }
    processCommand(c);
    c->writeIndex = 0;
    c->readIndex = 0;
    bzero(c->queryBuf, QUERY_BUFF_SIZE);
}

void processCommand(redisClient *c) {
    if(c->argv == NULL) {
        printf("command is emputy\n");
        return;
    }

    char *cmd = c->argv[0];
    if(strcasecmp(cmd,"set")== 0) {
        setCommand(c, c->argv);
    }else if(strcasecmp(cmd,"get")== 0) {
        getCommand(c, c->argv);
    }else if(strcasecmp(cmd,"del")== 0) {
        delCommand(c, c->argv);
    }
    c->argv = NULL;
    c->argc = 0;
}

void processMultiBulk(redisClient *c, char *data){
    int pos = 0;
    char *str = strchr(data,'\r');
    int len = str - data -1;
    char *cc = (char *)malloc(sizeof(char) * len);
    bzero(cc, len);
    memcpy(cc,data + 1,len);
    int bulkLen = atoi(cc);
    pos += 1 + len + 2;
    char *cur = NULL;
    char **argv = (char **)malloc(sizeof(char *) * bulkLen);
    c->argc = bulkLen;
    for(int i = 0; i < bulkLen; i++){
        if(data[pos] != '$'){
            perror("the protocol is incorrect.");
            exit(-1);
        }

        cur = data + pos;
        str = strchr(cur,'\r');
        len = str - cur -1;
        cc = (char *)malloc(sizeof(char) * len);
        bzero(cc, len);
        memcpy(cc,cur + 1,len);
        int strLen = atoi(cc);
        pos += 1 + len + 2;
        cc = (char *)malloc(sizeof(char) * strLen);
        bzero(cc, strLen);
        memcpy(cc,data + pos,strLen);

        argv[i] = cc;
        pos += strLen + 2;
    }
    c->argv = argv;
}

void processInline(redisClient *c, char *data){
    switch(data[0]){
        case '+':
            break;
        case '-':
            break;
        case '$':
            break;
        case ':':
            break;
        default:
            break;
    }
}



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
    dict *db = c->curDb;
    char *key = argv[1];

    dictEntry *entry = dictFind(db,key);

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
    dict *db = c->curDb;
    char *key = argv[1];
    char *value = argv[2];
    dictAdd(db, key, value);
    sendStr(c,"OK");
}

void expireCommand(redisClient *c, void ** argv) {

}

void delCommand(redisClient *c, void ** argv) {

}

void infoCommand(redisClient *c, void ** argv) {

}
