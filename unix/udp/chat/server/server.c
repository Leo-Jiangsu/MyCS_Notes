#include <header.h>

int main(int argc, char *argv[])
{
    int ret = 0;
    ARGS_CHECK(argc, 2);
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    ERROR_CHECK(sockfd, -1, "socket");
    
    struct sockaddr_in serAddr, cliAddr;
    memset(&serAddr, 0, sizeof(serAddr));
    memset(&cliAddr, 0, sizeof(serAddr));
    serAddr.sin_family = AF_INET;
    serAddr.sin_port = htons(atoi(argv[1]));
    serAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    /* ret = inet_pton(AF_INET, argv[1], &addr.sin_addr.s_addr); */ 
    /* ERROR_CHECK(ret, -1, "inet_pton"); */

    ret = bind(sockfd, (struct sockaddr *)&serAddr, sizeof(serAddr));
    ERROR_CHECK(ret, -1, "bind");
    
    //----------------------------------------
    char buf[64] = {0};
    socklen_t cliAddr_len;
    recvfrom(sockfd, buf, sizeof(buf), 0, (struct sockaddr *)&cliAddr, &cliAddr_len);

    fd_set readfds;
    printf("client ip = %s, port = %d\n", inet_ntoa(cliAddr.sin_addr), cliAddr.sin_port);
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
            sendto(sockfd, buf, strlen(buf) - 1, 0, 
                   (struct sockaddr *)&cliAddr, cliAddr_len);
        }
        if(FD_ISSET(sockfd, &readfds))
        {
            memset(buf, 0, sizeof(buf));
            recvfrom(sockfd, buf, sizeof(buf), 0,
                     (struct sockaddr *)&cliAddr, &cliAddr_len);
            printf("%s\n", buf);
        }
    }


    //----------------------------------------
    close(sockfd);
    return 0;
}

