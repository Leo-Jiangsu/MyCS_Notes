#include "include/threadpool.h"
#define MAXEVENTS 2

int exitpipe[2];

void sigfunc(int signum)
{

    char exitbuf = '1';
    write(exitpipe[1], &exitbuf, 1);

}

int main(int argc, char *argv[])
{
    //IP PORT threadcount
    ARGS_CHECK(argc, 4);

    pipe(exitpipe);
    if(fork())
    {
        signal(SIGUSR1, sigfunc);
        wait(NULL);
        exit(0);
    }

    Thread_pool_t tp;
    thread_pool_init(&tp, atoi(argv[3]));

    thread_pool_start(&tp);

    int sockfd = -1;
    tcp_init(argv[1], argv[2], &sockfd);

    int epfd = epoll_create(1);
    ERROR_CHECK(epfd, -1, "epoll_create");
    
    int ret = epoll_in_add(epfd, sockfd);
    ERROR_CHECK(ret, -1, "epoll_in_add");

    ret = epoll_in_add(epfd, exitpipe[0]);
    ERROR_CHECK(ret, -1, "epoll_in_add");

    struct epoll_event evs[MAXEVENTS];
    while(1)
    {
        int nready = epoll_wait(epfd, evs, MAXEVENTS, -1);
        for(int i = 0; i < nready; ++i)
        {
            if(evs[i].data.fd == sockfd)
            {
                int confd = accept(sockfd, NULL, NULL);
                pQueue_node_t pnew;
                queue_node_create(&pnew, confd);
                pthread_mutex_lock(&tp.taskQueue.mutex);
                task_enqueue(&tp.taskQueue, pnew);
                pthread_cond_signal(&tp.taskQueue.cond);
                pthread_mutex_unlock(&tp.taskQueue.mutex);
            }
            if(evs[i].data.fd == exitpipe[0])
            {

                tp.startFlag = 0;
                printf("exit\n");
                pthread_cond_broadcast(&tp.taskQueue.cond);
                for(int i = 0; i < tp.threadCnt; ++i)
                {
                    pthread_join(tp.pthid[i], NULL);
                }
                thread_pool_destory(&tp);
                close(sockfd);
                close(epfd);
                close(exitpipe[0]);
                close(exitpipe[1]);
                exit(0);
            }
        }
    }



    return 0;
}

void exitfunc()
{
}

