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
    sendfilepac(fd, confd);
    
    //mmap
    /* sendfilemmap(fd, confd, statbuf.st_size); */

    //sendfile
    /* sendfile(confd, fd, 0, statbuf.st_size); */

    //splice
    /* ret = sendfilesplice(fd, confd, statbuf.st_size); */
    /* ERROR_CHECK(ret, -1, "sendfilesplice"); */


    return 0;
}






