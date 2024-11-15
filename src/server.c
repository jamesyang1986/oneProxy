#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>

#include "net.h"
#include "server.h"
#include "redisCommand.h"
#include "log.h"

#define PORT 9527

Server *server;


int main(int argc, char* argv[]){
    // sprintf("net demo start...pid is: %d\n", getpid());
    server = (Server *)malloc(sizeof(*server));
    server->db = dictCreate(&dictTypeHeapStringCopyKey, NULL);
    server->verbosity = LOG_DEBUG;
    server->logfile = NULL;
    server->pid = getpid();

    Log(LOG_DEBUG, "net demo start...");

    int listenfd = socket_bind(IPADDRESS, PORT);
    server->listenfd = listenfd;
    listen(listenfd, LISTENQ);
    server->ev = aeCreateEventLoop(1024);
    aeCreateFileEvent(server->ev, listenfd, AE_READABLE, acceptTcpHandler, NULL);

    while(1){
        aeMain(server->ev);
    }
    aeStop(server->ev);
    return 0;
}