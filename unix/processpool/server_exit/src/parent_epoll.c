#include "../include/process_pool.h"
int parent_epoll_add(int epfd, int addfd)
{
    struct epoll_event event;
    event.data.fd = addfd;
    event.events = EPOLLIN;
    return epoll_ctl(epfd, EPOLL_CTL_ADD, addfd, &event);
}

int parent_epoll(int sockfd, process_data_t *childArr, int childProcessCount)
{
    int epfd = epoll_create(1);
    ERROR_CHECK(epfd, -1, "epoll_create");
    int ret;
#ifdef SERVEREXIT
    printf("exitpipe[0] = %d\n", exitpipe[0]);
    ret = parent_epoll_add(epfd, exitpipe[0]);
    ERROR_CHECK(ret, -1, "parent_epoll_add");
#endif

    ret = parent_epoll_add(epfd, sockfd);
    ERROR_CHECK(ret, -1, "parent_epoll_add");

    for(int i = 0; i < childProcessCount; ++i)
    {
        ret = parent_epoll_add(epfd, childArr[i].pipefd);
        ERROR_CHECK(ret, -1, "parent_epoll_add");
    }

    int nready = 0, confd = -1;
    /* struct epoll_event evs[childProcessCount + 1]; */
    struct epoll_event *evs = (struct epoll_event *)
        calloc(childProcessCount + 1, sizeof(struct epoll_event));
    while(1)
    {
        /* sleep(5); */
        nready = epoll_wait(epfd, evs, childProcessCount + 1, -1);
        for(int i = 0; i < nready; ++i)
        {
#ifdef SERVEREXIT
            if(evs[i].data.fd == exitpipe[0])
            {
                //exit
#if 0
                for(int i = 0; i < childProcessCount; ++i)
                {
                    kill(childArr[i].pid, SIGUSR1);
                    printf("child %d exit\n", childArr[i].pid);
                }
#else
                for(int i = 0; i < childProcessCount; ++i)
                {
                    sendfd(childArr[i].pipefd, 0, 1);

                }
#endif
                for(int i = 0; i < childProcessCount; ++i)
                {
                    printf("child %d exit\n", wait(NULL));
                }
                printf("server exit\n");
                exit(0);
            }
            else 
#endif
                
                if(evs[i].data.fd == sockfd)
            {
                //accept and send fd to child
                confd = accept(sockfd, NULL, NULL);
                ERROR_CHECK(confd, -1, "accept");
                for(int j = 0; j < childProcessCount; ++j)
                {
                    if(!childArr[j].isBusy)
                    {
                        ret = sendfd(childArr[j].pipefd, confd, 0);
                        ERROR_CHECK(ret, -1, "sendfd");
                        childArr[j].isBusy = 1;
                        break;
                    }
                }
                close(confd);
                confd = -1;
            }
            else
            {
                //child finish task
                printf("child finish task\n");
                for(int j = 0; j < childProcessCount; ++j)
                {
                    if(childArr[j].pipefd == evs[i].data.fd)
                    {
                        char isBusy;
                        ret = recv(childArr[j].pipefd, &isBusy, 1, 0);
                        ERROR_CHECK(ret, -1, "sendfd");
                        childArr[j].isBusy = 0;
                        break;
                    }
                }
            }
        }

    }


    return 0;
}

