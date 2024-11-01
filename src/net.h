#ifndef NET_H
#define NET_H

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/un.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <netdb.h>
#include <errno.h>
#include <stdarg.h>
#include <stdio.h>


#define MAXSIZE 1024
#define FDSIZE 1000
#define EPOLLEVENTS 100


// void do_epoll(int listenfd);

// void handle_events(int epollfd,
//                           struct epoll_event *events,
//                           int num, int listenfd, char *buf);

// void handle_accpet(int epollfd, int listenfd);

// void do_read(int epollfd, int fd, char *buf);

// void do_write(int epollfd, int fd, char *buf);

// void add_event(int epollfd, int fd, int state, void *client_data);

// void modify_event(int epollfd, int fd, int state);

// void delete_event(int epollfd, int fd, int state);


int socket_bind(const char *ip, int port);
int anetSetBlock(int fd, int non_block);

#endif
