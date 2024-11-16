//
// Created by james on 2024/11/16.
//

#ifndef ONEPROXY_CONNECTION_H
#define ONEPROXY_CONNECTION_H

#endif //ONEPROXY_CONNECTION_H

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
void reconnet();
int sendData(Conn *conn, char *data);
char *recvData(Conn *conn);

