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
    /* addr.sin_addr.s_addr = htonl(INADDR_ANY); */
    ret = inet_pton(AF_INET, argv[1], &addr.sin_addr.s_addr); 
    ERROR_CHECK(ret, -1, "inet_pton");
    /* addr.sin_addr.s_addr = htonl(addr.sin_addr.s_addr); */

    ret = connect(sockfd, (struct sockaddr *)&addr, sizeof(addr));
    ERROR_CHECK(ret, -1, "connect");
    
    //connect success
    //----------------------------------------

    char buf[64] = {0};

    fd_set readfds;
    while(1)
    {
        FD_ZERO(&readfds);
        FD_SET(STDIN_FILENO, &readfds);
        FD_SET(sockfd, &readfds);
        select(sockfd + 1, &readfds, NULL, NULL, NULL);
        if(FD_ISSET(STDIN_FILENO, &readfds))
        {
            memset(buf, 0, sizeof(buf));
            read(STDIN_FILENO, buf, sizeof(buf));
            send(sockfd, buf, strlen(buf) - 1, 0);
        }
        if(FD_ISSET(sockfd, &readfds))
        {
            memset(buf, 0, sizeof(buf));
            ret = recv(sockfd, buf, sizeof(buf), 0);
            if(0 == ret)
            {
                printf("chat end\n");
                break;
            }
            printf("%s\n", buf);
        }
    }



    //----------------------------------------
    close(sockfd);

    return 0;
}

