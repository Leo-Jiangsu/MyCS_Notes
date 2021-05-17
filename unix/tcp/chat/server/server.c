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
    /* printf("after listen\n"); */
    int confd = accept(sockfd, NULL, NULL);
    ERROR_CHECK(confd, -1, "accept");
    
    //connect success
    //----------------------------------------
    char buf[64] = {0};
    /* recv(confd, buf, sizeof(buf), 0); */ 
    /* printf("buf = %s\n", buf); */

    fd_set readfds;
    while(1)
    {
        FD_ZERO(&readfds);
        FD_SET(STDIN_FILENO, &readfds);
        FD_SET(confd, &readfds);
        select(confd + 1, &readfds, NULL, NULL, NULL);
        if(FD_ISSET(STDIN_FILENO, &readfds))
        {
            memset(buf, 0, sizeof(buf));
            read(STDIN_FILENO, buf, sizeof(buf));
            send(confd, buf, strlen(buf) - 1, 0);
        }
        if(FD_ISSET(confd, &readfds))
        {
            memset(buf, 0, sizeof(buf));
            ret = recv(confd, buf, sizeof(buf), 0);
            if(0 == ret)
            {
                printf("chat end\n");
                break;
            }
            printf("%s\n", buf);
        }
    }


    //----------------------------------------
    close(confd);
    close(sockfd);
    return 0;
}

