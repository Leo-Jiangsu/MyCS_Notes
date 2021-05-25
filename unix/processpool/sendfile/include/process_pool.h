#ifndef __PROCESS_POOL_H__
#define __PROCESS_POOL_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <unistd.h>
#include <dirent.h>
#include <time.h>
#include <pwd.h>
#include <grp.h>
#include <fcntl.h>
#include <signal.h>
#include <pthread.h>
#include <netdb.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <sys/select.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/msg.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define ARGS_CHECK(argc, val) \
    do {\
        if(argc != val)\
        {\
            fprintf(stderr, "args error!\n");\
            return -1;\
        }\
    }while(0)

#define ERROR_CHECK(ret, val, info) \
    do {\
        if(ret == val)\
        {\
            perror(info);\
            return -1;\
        }\
    }while(0)

#define THREAD_ERRORCHECK(ret, info) \
    do {\
        if(0 != ret)\
        {\
            fprintf(stderr, info ": %s:%d: %s\n", __FILE__, __LINE__, strerror(ret));\
            return -1;\
        }\
    }while(0)

#define CHILD_THREAD_ERRORCHECK(ret, info) \
    do {\
        if(0 != ret)\
        {\
            fprintf(stderr, info ": %s:%d: %s\n", __FILE__, __LINE__, strerror(ret));\
            return (void *)-1;\
        }\
    }while(0)


typedef struct 
{
    pid_t pid;
    int pipefd;//parent pipefd
    short isBusy;
}process_data_t;

typedef struct
{
    int data_len;
    char buf[1024];
}train;

#define FILENAME "file"

int create_child_process(process_data_t *childArr, int childProcessCount);
int parent_listen_socket(char *addr, char *port);
int parent_epoll_add(int epfd, int addfd);
int parent_epoll(int sockfd, process_data_t *childArr, int childProcessCount);
int recvfd(int pipefd, int *fd);
int sendfd(int pipefd, int fd);


#endif
