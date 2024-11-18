//
// Created by james on 2024/11/16.
//
#include "stdlib.h"
#include <stdint.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/errno.h>

#include "connection.h"
#include "net.h"
#include "log.h"

Conn *createConn(char *host, int port) {
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

Conn *reconnet(char *host, int port) {
    return NULL;
}

int sendData(Conn *c, char *buf, int len) {
    ssize_t send = 0;
    while (send < len) {
        int n = write(c->fd, buf + send, len - send);
        if (n == 0) {
            printf("send buff overflow\n");
            continue;
        }

        if (n == -1) {
            if (errno == EAGAIN) {
                printf("read fd: %d, EAGAIN\n", c->fd);
            }
            if (errno == EWOULDBLOCK) {
                printf("write fd:%d, EWOULDBLOCK\n", c->fd);
            }
            printf("send return -1,fail :%d, errno: %m\n", c->fd, errno);
            break;
        }
        send += n;
    }

    return len;
}

int recvData(Conn *c, char *buf, int len) {
    int n = read(c->fd, buf, len);
    if (n == 0) {

    }
    if (n == -1) {

    }
    return n;
}

void closeConn(Conn *c) {
    free(c);
}