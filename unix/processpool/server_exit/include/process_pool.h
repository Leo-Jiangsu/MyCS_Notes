#ifndef __PROCESS_POOL_H__
#define __PROCESS_POOL_H__

#define _GNU_SOURCE
#include <header.h>
typedef struct 
{
    pid_t pid;
    int pipefd;//parent pipefd
    short isBusy;
}process_data_t;

typedef struct
{
    size_t data_len;
    char buf[1024];
}train;

#define FILENAME "file"

#ifndef CLIENT
int create_child_process(process_data_t *childArr, int childProcessCount);
int parent_listen_socket(char *addr, char *port);
int parent_epoll_add(int epfd, int addfd);
int parent_epoll(int sockfd, process_data_t *childArr, int childProcessCount);
int recvfd(int pipefd, int *fd, int *isexit);
int sendfd(int pipefd, int fd, int isexit);

int sendpac(int confd, const train *data);
int sendbigfile(int fd, int confd);
extern int exitpipe[2];
#else

int recvbuf(int sockfd, void *buf, size_t len);
int recvpac(int sockfd, train *data);
int recvfile(int sockfd);

#endif

#define SERVEREXIT

#endif

