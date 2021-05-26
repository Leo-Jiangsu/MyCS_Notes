#ifndef __PROCESS_POOL_H__
#define __PROCESS_POOL_H__
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

int create_child_process(process_data_t *childArr, int childProcessCount);
int parent_listen_socket(char *addr, char *port);
int parent_epoll_add(int epfd, int addfd);
int parent_epoll(int sockfd, process_data_t *childArr, int childProcessCount);
int recvfd(int pipefd, int *fd);
int sendfd(int pipefd, int fd);

int sendpac(int confd, const train *data);
int sendbigfile(int fd, int confd);
int recvbuf(int sockfd, void *buf, size_t len);
int recvpac(int sockfd, train *data);
int recvfile(int sockfd);
#endif
