#include "../include/process_pool.h"

int sendpac(int confd, const train *data)
{
    int ret = send(confd, &data->data_len, sizeof(data->data_len), 0);
    ERROR_CHECK(ret, -1, "send");
    send(confd, data->buf, data->data_len, 0);
    ERROR_CHECK(ret, -1, "send");
    return 0;
}

int sendfile(int fd, int confd)
{
    train filename, data;
    memset(filename.buf, 0, sizeof(filename.buf));
    memset(data.buf, 0, sizeof(data.buf));
    
    filename.data_len = strlen(FILENAME);
    strcpy(filename.buf, FILENAME);
    int ret = sendpac(confd, &filename);
    ERROR_CHECK(ret, -1, "sendpac");
    
    while(1)
    {
        data.data_len = read(fd, data.buf, sizeof(data.data_len));
        sendpac(confd, &data);
        if(0 == data.data_len)
        {
            break;
        }
    }
    return 0;
}

int recvbuf(int sockfd, void *buf, size_t len)
{
    size_t totalrecv = 0;
    char *p = (char *)buf;
    while(totalrecv < len)
    {
        int ret = recv(sockfd, p, len - totalrecv, 0);
        ERROR_CHECK(ret, -1, "recv");
        totalrecv += ret;
        p += totalrecv;
    }
    return 0;
}

int recvpac(int sockfd, train *data)
{
    int ret = recvbuf(sockfd, &data->data_len, sizeof(data->data_len));
    ERROR_CHECK(ret, -1, "recvbuf");
    ret = recvbuf(sockfd, data->buf, data->data_len);
    ERROR_CHECK(ret, -1, "recvbuf");
    return 0;
}

int recvfile(int sockfd)
{
    train filename, data;
    memset(filename.buf, 0, sizeof(filename.buf));
    memset(data.buf, 0, sizeof(data.buf));
    
    int ret = recvpac(sockfd, &filename);
    ERROR_CHECK(ret, -1, "recvpac");
    int fd = open(filename.buf, O_RDWR|O_CREAT, 0666);
    ERROR_CHECK(fd, -1, "open");
    while(1)
    {
        ret = recvpac(sockfd, &data);
        ERROR_CHECK(ret, -1, "recvpac");
        if(0 == data.data_len)
        {
            break;
        }
        write(fd, data.buf, data.data_len);
    }
    return 0;
    close(fd);
}


