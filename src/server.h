#include "ae.h"
#include "dict.h"

#ifndef SERVER_H
#define SERVER_H

#define IPADDRESS "0.0.0.0"

#define LISTENQ 5
#define WORKER_SIZE 1
#define CONN_BUF_SIZE 1024


struct aeEventLoop;

typedef struct Server{
    int listenfd;
    int epollfd;
    struct aeEventLoop *ev;
    dict *db;
    int verbosity;
    char *logfile;
    pid_t pid;

}Server;

typedef struct Client{
    int clientfd;
    char buf[CONN_BUF_SIZE];
    int readIndex;
    int writeIndex;
    int (*connProc)(aeEventLoop *, int, void*, int);
    int (*readProc)(aeEventLoop *, int, void*, int);
    int (*writeProc)(aeEventLoop *, int, void*, int);
}Client;


#endif