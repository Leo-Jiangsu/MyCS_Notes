#include <header.h>

struct client
{
    int clientfd;
    struct sockaddr_in cliAddr;
    socklen_t addrlen;
    struct client *next;
};


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
    
    //----------------------------------------

    fd_set readfds, basefds;
    FD_ZERO(&readfds);
    FD_ZERO(&basefds);
    FD_SET(sockfd, &basefds);
    int maxfd = sockfd; 
    
    struct client clientList;   
    
    clientList.next = NULL;

    char buf[64] = {0};

    while(1)
    {
        memcpy(&readfds, &basefds, sizeof(fd_set));
        select(maxfd + 1, &readfds, NULL, NULL, NULL);

        if(FD_ISSET(sockfd, &readfds))
        {//new connection
            struct client *newClient = (struct client *)malloc(sizeof(struct client));
            newClient->next = clientList.next;
            clientList.next = newClient;
            newClient->clientfd = accept(sockfd,
                                         (struct sockaddr *)&newClient->cliAddr.sin_addr,
                                         &newClient->addrlen); 
            ERROR_CHECK(newClient->clientfd, -1, "accept");
            printf("%s connected\n", inet_ntoa(newClient->cliAddr.sin_addr));
            //update maxfd
            maxfd = (maxfd > newClient->clientfd) ? maxfd : newClient->clientfd;

            //update basefds 
            FD_SET(newClient->clientfd, &basefds);
        }
        struct client *tempClient = clientList.next;
        struct client *tempfront = &clientList;
        while(tempClient)
        {
            if(FD_ISSET(tempClient->clientfd, &readfds))
            {
                //recv data 
                memset(buf, 0, sizeof(buf));
                int ret = recv(tempClient->clientfd, buf, sizeof(buf), 0);
                if(0 == ret)
                {
                    //client close
                    close(tempClient->clientfd);
                    printf("%s exit\n", inet_ntoa(tempClient->cliAddr.sin_addr));
                    //remove from fd_set
                    FD_CLR(tempClient->clientfd, &basefds); 
                    //delete node
                    tempfront->next = tempClient->next;
                    free(tempClient);
                    tempClient = tempfront->next;
                    continue;
                }
                else
                {
                    //forward
                    struct client *temp = clientList.next;
                    while(temp)
                    {
                        if(temp != tempClient)
                        {
                            send(temp->clientfd, buf, strlen(buf), 0);
                        }
                        temp = temp->next;
                    }
                }
            }
            tempClient = tempClient->next;
            tempfront = tempfront->next;
        }
    }

    close(sockfd);
    //----------------------------------------
        
    return 0;
}


