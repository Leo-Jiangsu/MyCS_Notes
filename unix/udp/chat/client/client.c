#include <header.h>

int main(int argc, char *argv[])
{
    int ret = 0;
    ARGS_CHECK(argc, 3);
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    ERROR_CHECK(sockfd, -1, "socket");
    
    struct sockaddr_in serAddr;
    socklen_t serAddr_len;
    memset(&serAddr, 0, sizeof(serAddr));
    serAddr.sin_family = AF_INET;
    serAddr.sin_port = htons(atoi(argv[2]));
    /* addr.sin_addr.s_addr = htonl(INADDR_ANY); */
    ret = inet_pton(AF_INET, argv[1], &serAddr.sin_addr.s_addr);
    ERROR_CHECK(ret, -1, "inet_pton");
    /* serAddr.sin_addr.s_addr = htonl(serAddr.sin_addr.s_addr); */
    //----------------------------------------
    char buf[64] = {0};
    sendto(sockfd, "1", 1, 0, (struct sockaddr *)&serAddr, sizeof(serAddr));
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
            sendto(sockfd, buf, strlen(buf) - 1, 0, 
                   (struct sockaddr *)&serAddr, sizeof(serAddr));
        }
        if(FD_ISSET(sockfd, &readfds))
        {
            memset(buf, 0, sizeof(buf));
            recvfrom(sockfd, buf, sizeof(buf), 0,
                     (struct sockaddr *)&serAddr, &serAddr_len);
            printf("%s\n", buf);
        }
    }



    //----------------------------------------
    close(sockfd);

    return 0;
}

