#include <header.h>

int main(int argc, char *argv[])
{
    int ret = 0;
    ARGS_CHECK(argc, 3);
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    ERROR_CHECK(sockfd, -1, "socket");

    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(atoi(argv[2]));
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    /* ret = inet_pton(AF_INET, argv[1], &addr.sin_addr.s_addr); */ 
    /* ERROR_CHECK(ret, -1, "inet_pton"); */

    ret = bind(sockfd, (struct sockaddr *)&addr, sizeof(addr));
    ERROR_CHECK(ret, -1, "bind");

    ret = listen(sockfd, 5);
    ERROR_CHECK(ret, -1, "listen");
    
    
    int confd = -1;
    //connect success
    //----------------------------------------
    char buf[64] = {0};

    fd_set readfds, basefds;
    FD_ZERO(&readfds);
    FD_ZERO(&basefds);
    FD_SET(STDIN_FILENO, &basefds);
    FD_SET(sockfd, &basefds); 

    while(1)
    {
        /* FD_SET(confd, &readfds); */
        memcpy(&readfds, &basefds, sizeof(fd_set));
        select(10, &readfds, NULL, NULL, NULL);
        if(FD_ISSET(STDIN_FILENO, &readfds))
        {
            memset(buf, 0, sizeof(buf));
            read(STDIN_FILENO, buf, sizeof(buf));
            if(confd > 0)
            {
                send(confd, buf, strlen(buf) - 1, 0);
            }
        }
        if(FD_ISSET(sockfd, &readfds))
        {
            confd = accept(sockfd, NULL, NULL);
            ERROR_CHECK(confd, -1, "accept");
            FD_SET(confd, &basefds);
            printf("client connected\n");
        }
        if(FD_ISSET(confd, &readfds))
        {
            memset(buf, 0, sizeof(buf));
            ret = recv(confd, buf, sizeof(buf), 0);
            if(0 == ret)
            {
                //disconnect 
                printf("chat end\n");
                close(confd);
                FD_CLR(confd, &basefds);
                confd = -1;
            }
            else
            {
                printf("%s\n", buf);
            }
        }
        //----------------------------------------
    }
    close(sockfd);
    return 0;
}

