//
// Created by james on 24-11-13.
//

#include <sys/epoll.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/un.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <stdarg.h>
#include <stdio.h>

#include "net_epoll.h"

#define MAXSIZE 1024
#define FDSIZE 1000
#define EPOLLEVENTS 100

void add_event(int epollfd, int fd, int state, void *client_data) {
    struct epoll_event event;
    event.events = state;
    event.data.fd = fd;
    event.data.ptr = client_data;
    epoll_ctl(epollfd, EPOLL_CTL_ADD, fd, &event);
}

void modify_event(int epollfd, int fd, int state) {
    struct epoll_event event;
    event.events = state;
    event.data.fd = fd;
    epoll_ctl(epollfd, EPOLL_CTL_MOD, fd, &event);
}

void delete_event(int epollfd, int fd, int state) {
    struct epoll_event event;
    event.events = state;
    event.data.fd = fd;
    epoll_ctl(epollfd, EPOLL_CTL_DEL, fd, &event);
}


int create_epoll_fd(int listenfd, int fdsize){
    int epollfd = epoll_create(fdsize);
    add_event(epollfd, listenfd, EPOLLIN, NULL);
    return epollfd;
}


void handle_events(int epollfd, struct epoll_event *events,
                   int num, int listenfd, char *buf) {
    for (int i = 0; i < num; i++) {
        int fd = events[i].data.fd;
        if (fd == listenfd && (events[i].events & EPOLLIN))
            handle_accpet(epollfd, listenfd);
        else if (events[i].events & EPOLLIN)
            do_read(epollfd, fd, buf);
        else if (events[i].events & EPOLLOUT)
            do_write(epollfd, fd, buf);
    }
}


void handle_accpet(int epollfd, int listenfd) {
    int clifd;
    struct sockaddr_in cliaddr;
    socklen_t cliaddrlen;
    clifd = accept(listenfd, (struct sockaddr *)NULL, NULL);
    if (clifd < 0)
        perror("accept error");
    else {
        printf("acceot new client:%s:%d\n",
               inet_ntoa(cliaddr.sin_addr),
               cliaddr.sin_port);
        add_event(epollfd, clifd, EPOLLIN, NULL);
    }
}

void do_read(int epollfd, int fd, char *buf) {
    ssize_t nread = read(fd, buf, MAXSIZE);
    if (nread <= 0) {
        if (nread == 0)
            fprintf(stderr, "client close.\n");
        else
            perror("read error");
        close(fd);
        delete_event(epollfd, fd, EPOLLIN);
    } else {
        printf("read msg is: %s\n", buf);
        modify_event(epollfd, fd, EPOLLOUT);
    }
}

void do_write(int epollfd, int fd, char *buf) {
    printf("write msg is: %s", buf);
    ssize_t nwrite = write(fd, buf, strlen(buf));
    if (nwrite < 0) {
        perror("write error");
        close(fd);
        delete_event(epollfd, fd, EPOLLOUT);
    } else {
        modify_event(epollfd, fd, EPOLLIN);
        memset(buf, 0, MAXSIZE);
    }
}
