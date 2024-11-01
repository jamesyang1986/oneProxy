#include "ae.h"

#ifndef SERVER_H
#define SERVER_H

#define IPADDRESS "0.0.0.0"
#define PORT 9527
#define LISTENQ 5
#define WORKER_SIZE 1
#define CONN_BUF_SIZE 1024

typedef struct Server{
    int listenfd;
    int epollfd;
    struct aeEventLoop *ev;

}Server;

typedef struct Client{
    int clientfd;
    char buf[CONN_BUF_SIZE];
    int readIndex;
    int writeIndex;
    int (*readProc)(int, int, void*);
    int (*writeProc)(int, int, void*);
}Client;

#endif