#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>

#include "net.h"
#include "server.h"
#include "redisCommand.h"
#include "log.h"
#include "serverConfig.h"

Server *server;


Conn *getOrCreateConn(char *host, int port);

int main(int argc, char *argv[]) {
    server = (Server *) malloc(sizeof(*server));
    server->db = dictCreate(&dictTypeHeapStringCopyKey, NULL);
    server->verbosity = LOG_DEBUG;
    server->logfile = NULL;
    server->pid = getpid();
    server->startTime = time(NULL);

    ProxyConfig *proxyConfig = loadConfig("/tmp/test.conf");
    server->connMap = dictCreate(&dictTypeHeapStringCopyKey, NULL);
    Cluster *cluster = proxyConfig->cluster;
    for (int i = 0; i < cluster->nodeNum; i++) {
        Node *node = cluster->nodes[i];
        Instance *master = node->master;
        getOrCreateConn(master->host, master->port);
        Instance *slave = node->slave;
        getOrCreateConn(slave->host, slave->port);
    }

    server->proxyConfig = proxyConfig;

    Log(LOG_DEBUG, "net demo started, pid:%d", server->pid);

    server->listenfd = socket_bind(proxyConfig->listen, proxyConfig->port);
    if (server->listenfd < 0) {
        Log(LOG_ERROR, "fail to bind socket, exit.");
        exit(-1);
    }

    listen(server->listenfd, LISTENQ);
    server->ev = aeCreateEventLoop(EVENT_MAX_SIZE);
    aeCreateFileEvent(server->ev, server->listenfd, AE_READABLE, acceptTcpHandler, NULL);

    while (1) {
        aeMain(server->ev);
    }

    aeStop(server->ev);
    return 0;
}

Conn *getOrCreateConn(char *host, int port) {
    char endpoint[30];
    sprintf(endpoint, "%s:%d", host, port);
    Conn *conn = dictFetchValue(server->connMap, endpoint);
    if (conn != NULL)
        return conn;
    conn = createConn(host, port);
    if (conn == NULL) {
        Log(LOG_ERROR, "fail to create backend server conn");
        return NULL;
    }

    dictAdd(server->connMap, endpoint, conn);
    return conn;
}
