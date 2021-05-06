#include <header.h>
#include "header/func.h"
//b.c
static int fdw = -1;
static int fdr = -1;
static int msqid = -1;

void sigfunc_of_b(int signum, siginfo_t *info, void *p)
{
    printf("chat closed.\n");
    killpid(PID_B1_FILENAME);
    killpid(PID_A1_FILENAME);
    close(fdw);
    close(fdr);
    msgctl(msqid, IPC_RMID, NULL);
    killpid(PID_A_FILENAME);
    /* kill(getpid(), SIGQUIT); */
    exit(0);
}


int main()
{
    
    savepid(PID_B_FILENAME);
    //register signal function\TODO
    sigfunc_register(sigfunc_of_b); 
    
    
    int fdw = open("1.fifo", O_WRONLY);
    int fdr = open("2.fifo", O_RDONLY);
    int msqid = msgget(KEY_OF_MSQ, IPC_CREAT|0600);

    while(1)
    {
        fd_set readset;
        FD_ZERO(&readset);
        FD_SET(fdr, &readset);
        FD_SET(STDIN_FILENO, &readset);
        select(fdr + 1, &readset, NULL, NULL, NULL);
        if(FD_ISSET(fdr, &readset))
        {//rcv message
            char str[64] = {0};
            int ret = read(fdr, str, sizeof(str));
            if(0 == ret)
            {
                printf("chat closed.\n");
                return 0;
            }
            //snd msg to msgqueue
            //\TODO
            snd_msg_to_msq(str, msqid, RCVMSG);
        }
        if(FD_ISSET(STDIN_FILENO, &readset))
        {//snd message
            char str[64] = {0};
            read(STDIN_FILENO, str, sizeof(str));
            write(fdw, str, strlen(str));
            //snd msg to msgqueue
            //\TODO
            snd_msg_to_msq(str, msqid, SNDMSG);
        }
    }
    //\TODO

    /* sigfunc_of_b(SIGUSR1, NULL, NULL); */
    
    return 0;
}

