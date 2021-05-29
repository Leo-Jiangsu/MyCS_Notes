#include "recvfile.h"

void print_progress_bar(off_t cur, off_t total);

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

int recvpac(int sockfd, Filepac_t *pac)
{
    int ret = recvbuf(sockfd, &pac->paclen, sizeof(pac->paclen));
    ERROR_CHECK(ret, -1, "recvbuf");
    ret = recvbuf(sockfd, pac->buf, pac->paclen);
    ERROR_CHECK(ret, -1, "recvbuf");
    return 0;
}

int recvfile(int sockfd)
{
    //recv filename
    Filepac_t filename, pac;
    memset(filename.buf, 0, sizeof(filename.buf));
    memset(pac.buf, 0, sizeof(pac.buf));
    int ret = recvpac(sockfd, &filename);
    ERROR_CHECK(ret, -1, "recvpac");
    int fd = open(filename.buf, O_RDWR|O_CREAT, 0666);
    ERROR_CHECK(fd, -1, "open");
    //--------------------------
    //recv file size
    ret = recvpac(sockfd, &pac);
    ERROR_CHECK(ret, -1, "recvpac");
    off_t filesize, cursize = 0; 
    memcpy(&filesize, pac.buf, sizeof(pac.paclen));
    printf("file size = %ld\n", filesize);
    //--------------------------
    //recv file
    off_t printpoint = filesize / 1000;


    char buf[1024] = {0};
    while(cursize < filesize)
    {
        ret = recv(sockfd, buf, sizeof(buf), 0);
        if(0 == ret)
        {
            //sock disconnected
            close(fd);
            return 0;
        }
        write(fd, buf, ret);
        cursize += ret;
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

