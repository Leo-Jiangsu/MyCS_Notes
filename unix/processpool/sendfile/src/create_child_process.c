#include "../include/process_pool.h"

int child_handle(int pipefd)
{
    int confd = -1;
    while(1)
    {
        int ret = recvfd(pipefd, &confd);
        ERROR_CHECK(ret, -1, "recvfd");
        //send file to client
        //TODO
        int fd = open(FILENAME, O_RDWR);
        train data;
        //send filename
        data.data_len = strlen(FILENAME);
        strcpy(data.buf, FILENAME);
        ret = send(confd, &data, sizeof(int) + data.data_len, 0);
        ERROR_CHECK(ret, -1, "send");
        
        //send file
        data.data_len = read(fd, data.buf, sizeof(data.buf));
        ret = send(confd, &data, sizeof(int) + data.data_len, 0);
        ERROR_CHECK(ret, -1, "send");



        close(confd);
        //send isBusy to parent
        char isBusy = 0;
        send(pipefd, &isBusy, 1, 0);
    }

}


int create_child_process(process_data_t *childArr, int childProcessCount)
{
    int fds[2];
    for(int i = 0; i < childProcessCount; ++i)
    {
        int ret = socketpair(AF_LOCAL, SOCK_STREAM, 0, fds);
        ERROR_CHECK(ret, -1, "socketpair");

        int pid = 0;
        if( 0 == (pid = fork()) )
        {
            //child process
            close(fds[0]);
            ret = child_handle(fds[1]);
            ERROR_CHECK(ret, -1, "child_handle");
        }
        else
        {
            //parent process
            close(fds[1]);
            childArr[i].pid = pid;
            childArr[i].pipefd = fds[0];
            childArr[i].isBusy = 0;

        }
    }
    return 0;
}

