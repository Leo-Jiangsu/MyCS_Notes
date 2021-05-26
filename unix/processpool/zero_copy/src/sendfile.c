#define _GNU_SOURCE
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
void sendfilepac(int fd, int confd)
{
    train data;
    while(1)
    {
        data.data_len = read(fd, data.buf, sizeof(data.data_len));
        sendpac(confd, &data);
        if(0 == data.data_len)
        {
            break;
        }
    }
}
int recvfilepac(int fd, int sockfd, off_t filesize)
{
    off_t cursize = 0;

    off_t printpoint = filesize / 1000;
    train data;
    while(1)
    {
        int ret = recvpac(sockfd, &data);
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

void recvfilemmap(int fd, int sockfd, off_t filesize)
{
    ftruncate(fd, filesize);
    char *pmap = (char *)mmap(NULL, filesize, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
    recv(sockfd, pmap, filesize, 0);
    munmap(pmap, filesize);
}
void sendfilemmap(int fd, int confd, off_t filesize)
{
    char *pmap = (char *)mmap(NULL, filesize, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
    send(confd, pmap, filesize, 0);
    munmap(pmap, filesize);
}

int sendfilesplice(int fd, int confd, off_t filesize)
{
    int fds[2];
    pipe(fds);
    off_t sendsize = 0;
    while(sendsize < filesize)
    {
        int ret = splice(fd, NULL, fds[1], NULL, 65536, SPLICE_F_MORE);
        ERROR_CHECK(ret, -1, "splice");
        ret = splice(fds[0], NULL, confd, NULL, 65536, SPLICE_F_MORE);
        ERROR_CHECK(ret, -1, "splice");
        sendsize += ret;
    }

    close(fds[0]);
    close(fds[1]);
    return 0;
}
int recvfilesplice(int fd, int sockfd, off_t filesize)
{
    int fds[2];
    pipe(fds);
    off_t downloadsize = 0;
    while(downloadsize < filesize)
    {
        int ret = splice(sockfd, NULL, fds[1], NULL, 65536, SPLICE_F_MORE);
        ERROR_CHECK(ret, -1, "splice");
        ret = splice(fds[0], NULL, fd, NULL, 65536, SPLICE_F_MORE);
        ERROR_CHECK(ret, -1, "splice");
        downloadsize += ret;
    }

    close(fds[0]);
    close(fds[1]);
    return 0;
}

int sendbigfile(int fd, int confd)
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
    
    //send/recv
    /* sendfilepac(fd, confd); */
    
    //mmap
    /* sendfilemmap(fd, confd, statbuf.st_size); */

    //sendfile
    /* sendfile(confd, fd, 0, statbuf.st_size); */

    //splice
    ret = sendfilesplice(fd, confd, statbuf.st_size);
    ERROR_CHECK(ret, -1, "sendfilesplice");


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
    
    struct timeval start, end;
    gettimeofday(&start, NULL);


    ret = recvpac(sockfd, &data);
    ERROR_CHECK(ret, -1, "recvpac");
    off_t filesize = (off_t)data.buf; 
    memcpy(&filesize, data.buf, sizeof(off_t));
    //--------------------------
    //recv file

    //read/write
    /* recvfilepac(fd, sockfd, filesize); */
    
    //mmap
    /* recvfilemmap(fd, sockfd, filesize); */

    //splice
    ret = recvfilesplice(fd, sockfd, filesize);
    ERROR_CHECK(ret, -1, "sendfilesplice");

    
    gettimeofday(&end, NULL);
    printf("total time cost: %ld ms\n", (end.tv_sec - start.tv_sec) * 1000000 + end.tv_usec - start.tv_usec);

    close(fd);
    return 0;
}



