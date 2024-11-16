//
// Created by james on 2024/11/16.
//
#include "stdlib.h"
#include <stdint.h>
#include <stdio.h>

#include "connection.h"
#include "net.h"
#include "log.h"

Conn* createConn(char *host, int port) {
    Conn *conn = (Conn *) malloc(sizeof(Conn));
    conn->host = host;
    conn->port = port;
    char errInfo[256];
    int fd;
    if ((fd = anetTcpConnect(errInfo, host, port)) < 0) {
        Log(LOG_ERROR, "fail to connect target server %s:%d, errInfo:%s", host, port, errInfo);
        free(conn);
        return NULL;
    }

    conn->fd = fd;
    return conn;
}

void reconnet() {

}

int sendData(Conn *conn, char *data) {

    return 0;
}

char *recvData(Conn *conn) {
    return NULL;
}