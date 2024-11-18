//
// Created by james on 2024/11/16.
//

#ifndef ONEPROXY_CONNECTION_H
#define ONEPROXY_CONNECTION_H

typedef struct Connection{
    int fd;
    char * host;
    int port;
    int recvTimeout;
    int sendTimeout;
    char *sendBuf;
    char *recvBuf;

}Conn;

Conn* createConn(char *host, int port);
Conn* reconnet(char *host, int port);
int sendData(Conn *conn, char *buf, int size);
int recvData(Conn *conn, char *buf, int size);
void closeConn(Conn *c);

#endif //ONEPROXY_CONNECTION_H


