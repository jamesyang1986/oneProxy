



#ifndef SERVER_H
#define SERVER_H

#include <time.h>
#include "ae.h"
#include "dict.h"
#include "connection.h"
#include "serverConfig.h"

#define LISTENQ 5
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
    ProxyConfig *proxyConfig;
    dict *connMap;
    time_t startTime;
}Server;

#endif