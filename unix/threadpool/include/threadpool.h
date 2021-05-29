#ifndef __THREADPOOL_H__
#define __THREADPOOL_H__
#include "taskqueue.h"
#include "sendfile.h"
typedef struct 
{
    Task_queue_t taskQueue;
    pthread_t *pthid;
    int threadCnt;
    int startFlag;

}Thread_pool_t, *pThread_pool_t;

enum
{
    THREAD_POOL_ERROR = -1,
    THREAD_POOL_OK
};

int thread_pool_init(pThread_pool_t, int);
int thread_pool_start(pThread_pool_t);
int tcp_init(const char *, const char *, int *);
int epoll_in_add(int, int);
void *threadfunc(void *);
int thread_pool_destory(pThread_pool_t);

#define TASKQUECAPACITY 10
#define FILENAME "file"

#endif
