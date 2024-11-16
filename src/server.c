#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>

#include "net.h"
#include "server.h"
#include "redisCommand.h"
#include "log.h"

Server *server;

int main(int argc, char *argv[]) {
    server = (Server *) malloc(sizeof(*server));
    server->db = dictCreate(&dictTypeHeapStringCopyKey, NULL);
    server->verbosity = LOG_DEBUG;
    server->logfile = NULL;
    server->pid = getpid();

    Log(LOG_DEBUG, "net demo started, pid:%d", server->pid);

    server->listenfd = socket_bind(IPADDRESS, PORT);
    if (server->listenfd < 0) {
        Log(LOG_ERROR, "fail to bind socket, exit.");
        exit(-1);
    }

    listen(server->listenfd, LISTENQ);
    server->ev = aeCreateEventLoop(EVENT_MAX_SIZE);
    aeCreateFileEvent(server->ev, server->listenfd, AE_READABLE, acceptTcpHandler, NULL);

    Conn* conns[2];
    Conn *conn = createConn("127.0.0.1", 6379);
    if (conn == NULL) {
        Log(LOG_ERROR, "fail to create backend server conn");
        exit(-1);
    }

    conns[0] = conn;
    server->backConns = conns;

    while (1) {
        aeMain(server->ev);
    }

    aeStop(server->ev);
    return 0;
}