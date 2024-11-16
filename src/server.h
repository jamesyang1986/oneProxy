#include "ae.h"
#include "dict.h"

#ifndef SERVER_H
#define SERVER_H

#define IPADDRESS "0.0.0.0"

#define LISTENQ 5
#define PORT 9527
#define EVENT_MAX_SIZE 1024


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


#endif