#include "../include/process_pool.h"
int recvfd(int pipefd, int *fd, int *isexit)
{
    struct msghdr msg;
    memset(&msg, 0 , sizeof(msg));
    /* char isBusy = '1'; */
    struct iovec iov;
    iov.iov_base = isexit;
    iov.iov_len = sizeof(int);
    msg.msg_iov = &iov;
    msg.msg_iovlen = 1;
    size_t len = CMSG_LEN(sizeof(int));
    struct cmsghdr *cmsg = (struct cmsghdr *)calloc(len, 1);
    cmsg->cmsg_len = len;
    cmsg->cmsg_level = SOL_SOCKET;
    cmsg->cmsg_type = SCM_RIGHTS;
    msg.msg_control = cmsg;
    msg.msg_controllen = len;
    int ret = recvmsg(pipefd, &msg, 0);
    ERROR_CHECK(ret, -1, "sendmsg");
    
    *fd = *CMSG_DATA(cmsg);
    return ret;
}

int sendfd(int pipefd, int fd, int isexit)
{
    struct msghdr msg;
    memset(&msg, 0 , sizeof(msg));
    /* char isBusy = '1'; */
    struct iovec iov;
    iov.iov_base = &isexit;
    iov.iov_len = sizeof(int);
    msg.msg_iov = &iov;
    msg.msg_iovlen = 1;
    size_t len = CMSG_LEN(sizeof(int));
    struct cmsghdr *cmsg = (struct cmsghdr *)calloc(len, 1);
    cmsg->cmsg_len = len;
    cmsg->cmsg_level = SOL_SOCKET;
    cmsg->cmsg_type = SCM_RIGHTS;
    *CMSG_DATA(cmsg) = fd;
    msg.msg_control = cmsg;
    msg.msg_controllen = len;
    
    int ret = sendmsg(pipefd, &msg, 0);
    ERROR_CHECK(ret, -1, "sendmsg");
    return ret;
}

