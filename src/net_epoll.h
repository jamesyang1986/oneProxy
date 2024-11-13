//
// Created by james on 24-11-13.
//

#ifndef NET_EPOLL_H
#define NET_EPOLL_H

void do_epoll(int listenfd);

void handle_events(int epollfd, struct epoll_event *events, int num, int listenfd, char *buf);

void handle_accpet(int epollfd, int listenfd);

void do_read(int epollfd, int fd, char *buf);

void do_write(int epollfd, int fd, char *buf);

void add_event(int epollfd, int fd, int state, void *client_data);

void modify_event(int epollfd, int fd, int state);

void delete_event(int epollfd, int fd, int state);

#endif //NET_EPOLL_H
