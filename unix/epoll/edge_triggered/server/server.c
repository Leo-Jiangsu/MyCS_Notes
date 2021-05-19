#include <header.h>
#define MAXEVENTS 3

int tcp_server_connect(char *port)
{
    int ret = 0;
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    ERROR_CHECK(sockfd, -1 ,"socket");
    struct sockaddr_in sockAddr;
    sockAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    sockAddr.sin_family = AF_INET;
    sockAddr.sin_port = htons(atoi(port));


    //set socket option
    int reuse = 1;
    setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(int));
    //-----------------
    ret = bind(sockfd, (struct sockaddr *)&sockAddr, sizeof(sockAddr));
    ERROR_CHECK(ret, -1, "bind");

    listen(sockfd, 5);

    return sockfd;
}

int epoll_event_add(int epfd, int addfd, int mode)
{
    struct epoll_event readevent;
    readevent.events = mode;
    readevent.data.fd = addfd;
    return epoll_ctl(epfd, EPOLL_CTL_ADD, addfd, &readevent);
}

int epoll_event_del(int epfd, int addfd)
{
    struct epoll_event readevent;
    readevent.events = EPOLLIN;
    readevent.data.fd = addfd;
    return epoll_ctl(epfd, EPOLL_CTL_DEL, addfd, &readevent);
}
                
void set_fd_nonblock(int fd)
{
    int flag = fcntl(fd, F_GETFL);
    flag |= O_NONBLOCK;
    fcntl(fd, F_SETFL, flag);
}

int main(int argc, char *argv[])
{
    ARGS_CHECK(argc, 3);
    int sockfd = tcp_server_connect(argv[2]);
    ERROR_CHECK(sockfd, -1, "tcp_server_connect");

    

    int confd = -1;

    int epfd = epoll_create(1);
    struct epoll_event readyset[MAXEVENTS];

    int ret = epoll_event_add(epfd, STDIN_FILENO, EPOLLIN);
    ERROR_CHECK(ret, -1, "epoll_event_add");

    ret = epoll_event_add(epfd, sockfd, EPOLLIN);
    ERROR_CHECK(ret, -1, "epoll_event_add");
    
    char buf[5] = {0};

    while(1)
    {
        int nready = epoll_wait(epfd, readyset, MAXEVENTS, -1);
        for(int i = 0; i < nready; ++i)
        {
            if(readyset[i].data.fd == sockfd)
            {
                //connect
                confd = accept(sockfd, NULL, NULL);
                ERROR_CHECK(confd, -1, "accept");
                printf("chat connected.\n");
                //Edge Triggered
                ret = epoll_event_add(epfd, confd, EPOLLIN|EPOLLET);
                ERROR_CHECK(ret, -1, "epoll_event_add");
                //nonblock
                set_fd_nonblock(confd);
            }
            if(readyset[i].data.fd == STDIN_FILENO)
            {
                //send
                memset(buf, 0, sizeof(buf));
                read(STDIN_FILENO, buf, sizeof(buf));
                send(confd, buf, strlen(buf) - 1, 0);
            }
            if(readyset[i].data.fd == confd)
            {
                //recv
                while(1)
                {
                    memset(buf, 0, sizeof(buf));
                    ret = recv(confd, buf, sizeof(buf), 0);  
                    if(-1 == ret)
                    {
                        printf("\n");
                        break;
                    }
                    else if(0 == ret)
                    {
                        //chat end
                        ret = epoll_event_del(epfd, confd);
                        ERROR_CHECK(ret, -1, "epoll_event_del");
                        printf("chat end.\n");
                        close(confd);
                        confd = -1;
                        break;
                    }
                    else
                    {
                        printf("%s", buf);
                    }
                }
            }
        }
    }

    close(confd);
    close(sockfd);

    return 0;
}

