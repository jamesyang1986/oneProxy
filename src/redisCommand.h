//
// Created by james on 24-11-14.
//

#ifndef REDISCOMMAND_H
#define REDISCOMMAND_H

#include "redisCommand.h"
#include "dict.h"
#include "ae.h"

#define CONN_BUF_SIZE 1024
#define MAX 1024

#define QUERY_BUFF_SIZE 1024
#define SEND_BUFF_SIZE 1024#define MAX 1024

#define QUERY_BUFF_SIZE 1024
#define SEND_BUFF_SIZE 1024
#define THREAD_SIZE 20
#define THREAD_SIZE 20


#define PRO_INLINE 1
#define PRO_MULTIBULK 2

typedef struct redisClient{
    int fd;
    char *queryBuf;
    char *sendBuf;
    int connectNum;
    int argc;
    char **argv;
    int replOffset;
    dict *curDb;
    int reqType;

    int readIndex;
    int writeIndex;
    int (*connProc)(aeEventLoop *, int, void*, int);
    int (*readProc)(aeEventLoop *, int, void*, int);
    int (*writeProc)(aeEventLoop *, int, void*, int);

}redisClient;

void acceptTcpHandler(aeEventLoop *el, int fd, void *privdata, int mask);
void readQueryFromClient(aeEventLoop *el, int fd, void *clientData, int mask);

void getCommand(redisClient *c, void ** argv);

void setCommand(redisClient *c, void ** argv);

void expireCommand(redisClient *c, void ** argv);

void delCommand(redisClient *c, void ** argv);

void infoCommand(redisClient *c, void ** argv);


void processMultiBulk(redisClient *c, char *data);
void processInline(redisClient *c, char *data);
void processCommand(redisClient *c);


#endif //REDISCOMMAND_H
