#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "net.h"


// void add_event(int epollfd, int fd, int state, void *client_data) {
//     struct epoll_event event;
//     event.events = state;
//     event.data.fd = fd;
//     event.data.ptr = client_data;
//     epoll_ctl(epollfd, EPOLL_CTL_ADD, fd, &event);
// }

// void modify_event(int epollfd, int fd, int state) {
//     struct epoll_event event;
//     event.events = state;
//     event.data.fd = fd;
//     epoll_ctl(epollfd, EPOLL_CTL_MOD, fd, &event);
// }

// void delete_event(int epollfd, int fd, int state) {
//     struct epoll_event event;
//     event.events = state;
//     event.data.fd = fd;
//     epoll_ctl(epollfd, EPOLL_CTL_DEL, fd, &event);
// }


// int create_epoll_fd(int listenfd, int fdsize){
//     int epollfd = epoll_create(fdsize);
//     add_event(epollfd, listenfd, EPOLLIN, NULL);
//     return epollfd;
// }




// void handle_events(int epollfd, struct epoll_event *events,
//                    int num, int listenfd, char *buf) {
//     for (int i = 0; i < num; i++) {
//         int fd = events[i].data.fd;
//         if (fd == listenfd && (events[i].events & EPOLLIN))
//             handle_accpet(epollfd, listenfd);
//         else if (events[i].events & EPOLLIN)
//             do_read(epollfd, fd, buf);
//         else if (events[i].events & EPOLLOUT)
//             do_write(epollfd, fd, buf);
//     }
// }


// void handle_accpet(int epollfd, int listenfd) {
//     int clifd;
//     struct sockaddr_in cliaddr;
//     socklen_t cliaddrlen;
//     clifd = accept(listenfd, (struct sockaddr *)NULL, NULL);
//     if (clifd < 0)
//         perror("accept error");
//     else {
//         printf("acceot new client:%s:%d\n",
//                inet_ntoa(cliaddr.sin_addr),
//                cliaddr.sin_port);
//         add_event(epollfd, clifd, EPOLLIN, NULL);
//     }
// }

// void do_read(int epollfd, int fd, char *buf) {
//     ssize_t nread = read(fd, buf, MAXSIZE);
//     if (nread <= 0) {
//         if (nread == 0)
//             fprintf(stderr, "client close.\n");
//         else
//             perror("read error");
//         close(fd);
//         delete_event(epollfd, fd, EPOLLIN);
//     } else {
//         printf("read msg is: %s\n", buf);
//         modify_event(epollfd, fd, EPOLLOUT);
//     }
// }

// void do_write(int epollfd, int fd, char *buf) {
//     printf("write msg is: %s", buf);
//     ssize_t nwrite = write(fd, buf, strlen(buf));
//     if (nwrite < 0) {
//         perror("write error");
//         close(fd);
//         delete_event(epollfd, fd, EPOLLOUT);
//     } else {
//         modify_event(epollfd, fd, EPOLLIN);
//         memset(buf, 0, MAXSIZE);
//     }
// }



int socket_bind(const char *ip, int port) {
    int listenfd = socket(AF_INET, SOCK_STREAM, 0);
    if (listenfd < 0) {
        return -1;
    }

    struct sockaddr_in servaddr;
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    inet_pton(AF_INET, ip, &servaddr.sin_addr);
    servaddr.sin_port = htons(port);
    if (bind(listenfd, (struct sockaddr *) &servaddr, sizeof(servaddr)) < 0) {
        return -1;
    }
    return listenfd;
}


int anetSetBlock(int fd, int non_block) {
    int flags;
    if ((flags = fcntl(fd, F_GETFL)) == -1) {
        return -1;
    }

    if (non_block)
        flags |= O_NONBLOCK;
    else
        flags &= ~O_NONBLOCK;

    if (fcntl(fd, F_SETFL, flags) == -1) {
        return -1;
    }
    return 0;
}


