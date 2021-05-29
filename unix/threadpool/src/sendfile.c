#include "../include/threadpool.h"
#include "../include/sendfile.h"

int sendpac(int confd, Filepac_t *pac)
{
    int ret = send(confd, &pac->paclen, sizeof(pac->paclen), 0);
    ERROR_CHECK(ret, -1, "send");
    ret = send(confd, &pac->buf, pac->paclen, 0);
    ERROR_CHECK(ret, -1, "send");

    return THREAD_POOL_OK;
}

int send_file(int confd)
{
    int fd = open(FILENAME, O_RDWR);
    ERROR_CHECK(fd, -1, "open");
    //send FILENAME
    Filepac_t pac;
    pac.paclen = strlen(FILENAME);
    strcpy(pac.buf, FILENAME);
    int ret = sendpac(confd, &pac);
    ERROR_CHECK(ret, -1, "sendpac");
    
    //send file size
    struct stat filestat;
    fstat(fd, &filestat);
    memcpy(pac.buf, &filestat.st_size, sizeof(filestat.st_size));
    pac.paclen = sizeof(filestat.st_size);
    ret = sendpac(confd, &pac);
    ERROR_CHECK(ret, -1, "sendpac");

    //send file (mmap)
    char *pmap = (char *)mmap(NULL, filestat.st_size, PROT_READ, MAP_SHARED, fd, 0);
    ERROR_CHECK(pmap, (char *)-1, "mmap");

    ret = send(confd, pmap, filestat.st_size, 0);
    ERROR_CHECK(ret, -1, "send");

    munmap(pmap, filestat.st_size);

    close(fd);

    return THREAD_POOL_OK;
}

