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

#define QUERY_BUFF_SIZE 4*1024
#define SEND_BUFF_SIZE 1024
#define THREAD_SIZE 20
#define THREAD_SIZE 20


#define PRO_INLINE 1
#define PRO_MULTIBULK 2

typedef struct redisClient {
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

    int (*connProc)(aeEventLoop *, int, void *, int);

    int (*readProc)(aeEventLoop *, int, void *, int);

    int (*writeProc)(aeEventLoop *, int, void *, int);

} redisClient;

void acceptTcpHandler(aeEventLoop *el, int fd, void *privdata, int mask);

void readQueryFromClient(aeEventLoop *el, int fd, void *clientData, int mask);


void infoCommand(redisClient *c);

void processMultiBulk(redisClient *c, char *data);

void processInline(redisClient *c, char *data);

void processCommand(redisClient *c);

void sendERR(redisClient *c, char *errorInfo);
void sendStr(redisClient *c, char *data);
void sendNil(redisClient *c);
void sendBulkStr(redisClient *c, char *data);




#endif //REDISCOMMAND_H
