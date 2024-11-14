#ifndef NET_H
#define NET_H

int socket_bind(const char *ip, int port);
int anetSetBlock(int fd, int non_block);

#endif
