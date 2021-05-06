#include "header/func.h"

static int shmid = -1;
static int semid = -1;

void sigfunc_of_a1(int signum, siginfo_t *info, void *p)
{
    printf("chat closed.\n");
    killpid(PID_B1_FILENAME);
    shmctl(shmid, IPC_RMID, NULL);
    semctl(semid, IPC_RMID, 0);
    killpid(PID_A_FILENAME);
    killpid(PID_B_FILENAME);
    exit(0);
}


int main()
{
    savepid(PID_A1_FILENAME);
    //register signal function
    sigfunc_register(sigfunc_of_a1); 

    shmid = shmget(KEY_OF_SHM, 4096, IPC_CREAT|0600);
    msgbuf_t *p = (msgbuf_t *)shmat(shmid, NULL, 0);
    semid = semget(KEY_OF_SEM, 2, IPC_CREAT|0600);
    while(1)
    {
        print_msg_from_shm(semid, p);
    }
    return 0;
}

