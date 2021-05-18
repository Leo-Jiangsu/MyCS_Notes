#include <header.h>
#define MAXEVENTS 3
int main(int argc, char *argv[])
{
    ARGS_CHECK(argc, 3);
    int ret = 0;
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    ERROR_CHECK(sockfd, -1 ,"socket");
    struct sockaddr_in sockAddr;
    sockAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    sockAddr.sin_family = AF_INET;
    sockAddr.sin_port = htons(atoi(argv[2]));

    int reuse = 1;
    setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(int));

    ret = bind(sockfd, (struct sockaddr *)&sockAddr, sizeof(sockAddr));
    ERROR_CHECK(ret, -1, "bind");

    listen(sockfd, 5);

    
    int confd = -1;

    int epfd = epoll_create(1);
    struct epoll_event readevent, readyset[MAXEVENTS];
    readevent.events = EPOLLIN;
    readevent.data.fd = STDIN_FILENO;
    epoll_ctl(epfd, EPOLL_CTL_ADD, STDIN_FILENO, &readevent);


    readevent.events = EPOLLIN;
    readevent.data.fd = sockfd;
    epoll_ctl(epfd, EPOLL_CTL_ADD, sockfd, &readevent);
    
    
    char buf[64] = {0};

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

                readevent.events = EPOLLIN;
                readevent.data.fd = confd;
                epoll_ctl(epfd, EPOLL_CTL_ADD, confd, &readevent);

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
                memset(buf, 0, sizeof(buf));
                ret = recv(confd, buf, sizeof(buf), 0);  
                if(0 == ret)
                {
                    //chat end
                    //TODO
                    readevent.events = EPOLLIN;
                    readevent.data.fd = confd;
                    epoll_ctl(epfd, EPOLL_CTL_DEL, confd, &readevent);
                    printf("chat end.\n");
                    close(confd);
                    confd = -1;
                }
                else
                {
                    printf("%s\n", buf);
                }
            }
        }
    }

    close(confd);
    close(sockfd);

    return 0;
}

