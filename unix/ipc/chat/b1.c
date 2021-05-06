#include <header.h>
#include "header/func.h"
static int msqid = -1;

void sigfunc_of_b1(int signum, siginfo_t *info, void *p)
{
    printf("chat closed.\n");
    killpid(PID_A1_FILENAME);
    msgctl(msqid, IPC_RMID, NULL);
    killpid(PID_B_FILENAME);
    killpid(PID_A_FILENAME);
    /* kill(getpid(), SIGQUIT); */
    exit(0);
}


int main()
{
    savepid(PID_B1_FILENAME);
    
    sigfunc_register(sigfunc_of_b1);

    msqid = msgget(KEY_OF_MSQ, IPC_CREAT|0600);
    while(1)
    {
        print_msg_from_msq(msqid);
        
    }
    return 0;
}

