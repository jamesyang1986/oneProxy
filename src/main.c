#include <stdio.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>

#include "net_epoll.h"
#include "net.h"


#define WORKER_SIZE 1
#define LISTENQ 10

int main() {
    printf("Hello, World!\n");

    int listenfd = socket_bind("0.0.0.0", 9527);

     int pid_arr[WORKER_SIZE];
     for(int i = 0; i< WORKER_SIZE; i++){
         int cid = 0;
         if((cid = fork()) == 0){
             printf("in child process...pid is %d, ppid is %d, the listenfd is %d\n", getpid(), getppid(), listenfd);
             listen(listenfd, LISTENQ);
             do_epoll(listenfd);
         }else if(cid < 0){
             printf("fork error...\n");
             return -1;
         }

         pid_arr[i] = cid;
     }

     int status;
//     waitpid(-1, &status, WNOHANG);
     printf("exit the  parent process...\n");
    return 0;
}
