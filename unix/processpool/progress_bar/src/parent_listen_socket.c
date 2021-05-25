#include "../include/process_pool.h"

int parent_listen_socket(char *addr, char *port)
{
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    ERROR_CHECK(sockfd, -1, "socket");

    int use = 1;
    setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &use, sizeof(int));

    struct sockaddr_in serAddr;
    serAddr.sin_family = AF_INET;
    
    inet_pton(AF_INET, addr, &serAddr.sin_addr);
    serAddr.sin_port = htons(atoi(port));

    int ret = bind(sockfd, (struct sockaddr *)&serAddr, sizeof(serAddr));
    ERROR_CHECK(ret, -1, "bind");
    
    ret = listen(sockfd, 5);
    ERROR_CHECK(ret, -1, "listen");

    return sockfd;
}


