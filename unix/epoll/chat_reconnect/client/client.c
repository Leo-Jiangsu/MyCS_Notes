#include <header.h>
#define MAXEVENTS 2
int main(int argc, char *argv[])
{
    ARGS_CHECK(argc, 3);
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    ERROR_CHECK(sockfd, -1, "socket");
    
    struct sockaddr_in serAddr;
    serAddr.sin_addr.s_addr = inet_addr(argv[1]);
    serAddr.sin_family = AF_INET;
    serAddr.sin_port = htons(atoi(argv[2]));

    int ret = connect(sockfd, (struct sockaddr *)&serAddr, sizeof(serAddr));
    ERROR_CHECK(ret, -1, "connect");

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
            if(readyset[i].data.fd == STDIN_FILENO)
            {
                //send
                memset(buf, 0, sizeof(buf));
                read(STDIN_FILENO, buf, sizeof(buf));
                send(sockfd, buf, strlen(buf) - 1, 0);

            }
            if(readyset[i].data.fd == sockfd)
            {
                //recv
                memset(buf, 0, sizeof(buf));
                ret = recv(sockfd, buf, sizeof(buf), 0);  
                if(0 == ret)
                {
                    //chat end
                    close(sockfd);
                    return 0;                
                }
                else
                {
                    printf("%s\n", buf);
                }
            }
        }
    }

    close(sockfd);

    return 0;
}

