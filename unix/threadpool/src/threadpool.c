#include "../include/threadpool.h"

int thread_pool_init(pThread_pool_t pTP, int threadCnt)
{
    if(!pTP)
    {
        return THREAD_POOL_ERROR;
    }
    memset(pTP, 0, sizeof(Thread_pool_t));
    int ret = task_queue_init(&pTP->taskQueue, TASKQUECAPACITY);
    if(ret != TASK_QUEUE_OK)
    {
        return THREAD_POOL_ERROR;
    }
    pTP->pthid = (pthread_t *)calloc(sizeof(pthread_t), threadCnt);
    pTP->threadCnt = threadCnt;
    pTP->startFlag = 0;

    return THREAD_POOL_OK;
}

void pthread_cleanup_func(void *p)
{
    pThread_pool_t pTP = (pThread_pool_t)p;
    pthread_mutex_unlock(&pTP->taskQueue.mutex);
}

void *threadfunc(void *arg)
{
    pThread_pool_t pTP = (pThread_pool_t)arg;
    pthread_cleanup_push(pthread_cleanup_func, pTP);
    while(1)
    {
        pthread_mutex_lock(&pTP->taskQueue.mutex);
        /* if(!pTP->startFlag && task_queue_isempty(&pTP->taskQueue)) */
        if(!pTP->startFlag)
        {
            pthread_exit(NULL);
        }
        if(task_queue_isempty(&pTP->taskQueue))
        {
            printf("thread wait\n");
            pthread_cond_wait(&pTP->taskQueue.cond, &pTP->taskQueue.mutex);
        }
        int confd = -1;
        int ret = task_dequeue(&pTP->taskQueue, &confd);
        pthread_mutex_unlock(&pTP->taskQueue.mutex);
        if(ret == TASK_QUEUE_OK)
        {
            printf("thread wakeup\n");
            send_file(confd);
        }
    }
    return (void *)THREAD_POOL_OK;
    pthread_cleanup_pop(1);
}

int thread_pool_start(pThread_pool_t pTP)
{
    if(pTP->startFlag != 0)
    {
        return THREAD_POOL_ERROR;
    }
    pTP->startFlag = 1;
    for(int i = 0; i < pTP->threadCnt; ++i)
    {
        pthread_create(pTP->pthid + i, NULL, threadfunc, pTP);
    }
    return THREAD_POOL_OK;

}

int tcp_init(const char *ipaddr, const char *port, int *sockfd)
{
    int sfd = socket(AF_INET, SOCK_STREAM, 0);
    ERROR_CHECK(sfd, -1, "socket");

    struct sockaddr_in serAddr;
    serAddr.sin_family = AF_INET;
    inet_pton(AF_INET, ipaddr, &serAddr.sin_addr);
    serAddr.sin_port = htons(atoi(port));

    int ret = bind(sfd, (struct sockaddr *)&serAddr, sizeof(serAddr));
    ERROR_CHECK(ret, -1, "bind");

    ret = listen(sfd, 10);
    ERROR_CHECK(ret, -1, "listen");

    *sockfd = sfd;

    return THREAD_POOL_OK;
}

int epoll_in_add(int epfd, int fd)
{
    struct epoll_event event;
    event.data.fd = fd;
    event.events = EPOLLIN;
    int ret = epoll_ctl(epfd, EPOLL_CTL_ADD, fd, &event);
    ERROR_CHECK(ret, -1, "epoll_ctl");
    return THREAD_POOL_OK;
}


int thread_pool_destory(pThread_pool_t pTP)
{
    free(pTP->pthid);
    
    int ret = task_queue_destroy(&pTP->taskQueue);
    ERROR_CHECK(ret, -1, "task_queue_destroy");

    return THREAD_POOL_OK;

}


