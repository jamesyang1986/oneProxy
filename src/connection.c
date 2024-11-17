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

void reconnet() {

}

int sendData(Conn *c, char *buf) {
    int len = strlen(buf);
    ssize_t send = 0;
    while (send < len) {
        int n = write(c->fd, buf + send, len - send);
        if (n == 0) {
            printf("send buff overflow\n");
            continue;
        }

        if (n == -1) {
            if (errno == EAGAIN) {
                printf("EAGAIN\n");
            }
            if (errno == EWOULDBLOCK) {
                printf("EWOULDBLOCK\n");
            }
            printf("send return -1,fail :%d, errno: %m\n", c->fd, errno);
            break;
        }
        send += n;
    }

    return len;
}

int recvData(Conn *c, char *buf) {
    int len = strlen(buf);
    int size = 0;
    int n = read(c->fd, buf + size, len - size);
    size = n;
//    while (size < len) {
//        int n = read(c->fd, buf + size, len - size);
//        if (n == 0) {
//            continue;
//        }
//        if (n == -1) {
//            break;
//        }
//        size += n;
//    }

    return size;
}