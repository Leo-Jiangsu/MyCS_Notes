#include "../include/process_pool.h"

void print_progress_bar(off_t cur, off_t total);
int sendpac(int confd, const train *data)
{
    int ret = send(confd, &data->data_len, sizeof(data->data_len), 0);
    ERROR_CHECK(ret, -1, "send");
    send(confd, data->buf, data->data_len, 0);
    ERROR_CHECK(ret, -1, "send");
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

int sendfile(int fd, int confd)
{
    train filename, data;
    memset(filename.buf, 0, sizeof(filename.buf));
    memset(data.buf, 0, sizeof(data.buf));

    //send filename
    filename.data_len = strlen(FILENAME);
    strcpy(filename.buf, FILENAME);
    int ret = sendpac(confd, &filename);
    ERROR_CHECK(ret, -1, "sendpac");
    //--------------------------
    //send file size
    struct stat statbuf;
    fstat(fd, &statbuf);
    memcpy(data.buf, &statbuf.st_size, sizeof(statbuf.st_size));
    data.data_len = sizeof(statbuf.st_size); 
    ret = sendpac(confd, &data);
    ERROR_CHECK(ret, -1, "sendpac");
    //--------------------------
    //send file
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
int recvfile(int sockfd)
{
    //recv filename
    train filename, data;
    memset(filename.buf, 0, sizeof(filename.buf));
    memset(data.buf, 0, sizeof(data.buf));
    int ret = recvpac(sockfd, &filename);
    ERROR_CHECK(ret, -1, "recvpac");
    int fd = open(filename.buf, O_RDWR|O_CREAT, 0666);
    ERROR_CHECK(fd, -1, "open");
    //--------------------------
    //recv file size
    ret = recvpac(sockfd, &data);
    ERROR_CHECK(ret, -1, "recvpac");
    off_t filesize = (off_t)data.buf, cursize = 0; 
    memcpy(&filesize, data.buf, sizeof(off_t));
    //--------------------------
    //recv file

    off_t printpoint = filesize / 1000;
    while(1)
    {
        ret = recvpac(sockfd, &data);
        ERROR_CHECK(ret, -1, "recvpac");
        if(0 == data.data_len)
        {
            break;
        }
        cursize += data.data_len;
        write(fd, data.buf, data.data_len);
        if(cursize > printpoint)
        {
            print_progress_bar(cursize, filesize);
            printpoint += filesize / 1000;
        }
    }
    
    print_progress_bar(filesize, filesize);
    printf("\n");
    
    close(fd);
    return 0;
}


void print_progress_bar(off_t cur, off_t total)
{
    printf("\r[");
    off_t j = 50 * cur / total;
    for(off_t i = 0; i < j; ++i)
    {
        printf(">");
    }
    printf("%*s]%3.2f%%", (int)(50 - j), "",  (float)cur / total * 100);
    /* printf( */
    /*        "%*s]%3.2f%%", 20 - (int)(cur / total * 20), "-", (float)cur / total * 100); */
    fflush(stdout);
}

