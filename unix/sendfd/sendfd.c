#include <header.h>

int sendfd(int fd, int fdmsg)
{
    struct msghdr msg;
    memset(&msg, 0, sizeof(msg));

    char buf[64] = "hello";

    struct iovec iov;
    iov.iov_base = buf;
    iov.iov_len = strlen(buf);
    /* strcpy((char *)iov.iov_base, "hello"); */
    /* iov.iov_base = "hello"; */
    
    msg.msg_iov = &iov;
    msg.msg_iovlen = 1;

    int len = CMSG_LEN(sizeof(int));
    struct cmsghdr *cmsg = (struct cmsghdr *)calloc(1, len);
    cmsg->cmsg_len = len;
    cmsg->cmsg_level = SOL_SOCKET;
    cmsg->cmsg_type = SCM_RIGHTS;
    *(int *)CMSG_DATA(cmsg)  = fdmsg;

    msg.msg_control = cmsg;
    msg.msg_controllen = len;

    int ret = sendmsg(fd, &msg, 0);
    ERROR_CHECK(ret, -1, "sendmsg");

    /* free(iov.iov_base); */
    free(cmsg);
    return 0;
}

int recvfd(int fd, int *fdmsg)
{
    struct msghdr msg;
    memset(&msg, 0, sizeof(msg));

    char buf[64] = "hello";

    struct iovec iov;
    iov.iov_base = buf;
    iov.iov_len = strlen(buf);
    /* strcpy((char *)iov.iov_base, "hello"); */
    /* iov.iov_base = "hello"; */
    
    msg.msg_iov = &iov;
    msg.msg_iovlen = 1;

    int len = CMSG_LEN(sizeof(int));
    struct cmsghdr *cmsg = (struct cmsghdr *)calloc(1, len);
    cmsg->cmsg_len = len;
    cmsg->cmsg_level = SOL_SOCKET;
    cmsg->cmsg_type = SCM_RIGHTS;

    msg.msg_control = cmsg;
    msg.msg_controllen = len;

    /* printf("c 1\n"); */
    int ret = recvmsg(fd, &msg, 0);
    ERROR_CHECK(ret, -1, "sendmsg");
    
    *fdmsg = *(int *)CMSG_DATA(cmsg);

    /* printf("c 2\n"); */
    /* free(iov.iov_base); */
    free(cmsg);
    return 0;

}

int main(int argc, char *argv[])
{
    int fds[2];
    int ret = socketpair(AF_LOCAL, SOCK_STREAM, 0, fds);
    ERROR_CHECK(ret, -1, "socketpair");

    if(fork())
    {
        close(fds[0]);
        int fd = open("file", O_RDWR);
        write(fd, "helloworld", 10);
        lseek(fd, 0, SEEK_SET);
        ret = sendfd(fds[1], fd);
        ERROR_CHECK(ret, -1, "sendfd");
        wait(NULL);
        close(fds[1]);
        close(fd);
    }
    else
    {
        close(fds[1]);
        /* sleep(5); */
        int fd;
        recvfd(fds[0], &fd);
        ERROR_CHECK(fd, -1, "sendfd");
        printf("fd = %d\n", fd);

        char buf[64] = {0};
        read(fd, buf, sizeof(buf));
        printf("buf = %s\n", buf);
        close(fds[1]);
        close(fd);
    }


    return 0;
}

