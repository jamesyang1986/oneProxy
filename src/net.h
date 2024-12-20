#ifndef NET_H
#define NET_H

#define ANET_ERR_LEN 256
#define ANET_ERR -1
#define ANET_OK 1

int socket_bind(const char *ip, int port);
int anetSetBlock(int fd, int non_block);
int anetNonBlock(char *err, int fd);
int anetTcpConnect(char *err, char *addr, int port);
int anetTcpNonBlockConnect(char *err, char *addr, int port);
int anetUnixConnect(char *err, char *path);
int anetUnixNonBlockConnect(char *err, char *path);
int anetSendTimeout(char *err, int fd, long long ms);
int anetReceiveTimeout(char *err, int fd, long long ms);
int anetSetRecvBuffer(char *err, int fd, int buffsize);
int anetSetSendBuffer(char *err, int fd, int buffsize);
int anetEnableTcpNoDelay(char *err, int fd);
int anetTcpKeepAlive(char *err, int fd);


#endif
