#include <header.h>
#include "header/func.h"
//a.c
static int shmid = -1;
static int semid = -1;
static int fd_pid = -1; 
static int fdr = -1; 
static int fdw = -1; 

void sigfunc_of_a(int signum, siginfo_t *info, void *p)
{
    printf("chat closed.\n");
    killpid(PID_A1_FILENAME);
    killpid(PID_B1_FILENAME);
    shmctl(shmid, IPC_RMID, NULL);
    semctl(semid, IPC_RMID, 0);
    killpid(PID_B_FILENAME);
    /* kill(getpid(), SIGQUIT); */
    exit(0);
}

int main()
{
    savepid(PID_A_FILENAME);
    //register signal function
    
    sigfunc_register(sigfunc_of_a); 
    
    fd_pid = open(PIDFILENAME, O_RDWR);    
    fdr = open("1.fifo", O_RDONLY);
    ERROR_CHECK(fdr, -1, "open"); 
    fdw = open("2.fifo", O_WRONLY);
    ERROR_CHECK(fdw, -1, "open");

    shmid = shmget(KEY_OF_SHM, 4096, IPC_CREAT|0600);
    msgbuf_t *p = (msgbuf_t *)shmat(shmid, NULL, 0);
    semid = semget(KEY_OF_SEM, 2, IPC_CREAT|0600);
    short arr[2] = {1, 0};// arr[0]:room  arr[1]:message
    semctl(semid, 0, SETALL, arr);//represent count of message

    while(1)
    {
        fd_set readset;
        FD_ZERO(&readset);
        FD_SET(fdr, &readset);
        FD_SET(STDIN_FILENO, &readset);
        select(fdw + 1, &readset, NULL, NULL, NULL);
        if(FD_ISSET(fdr, &readset))
        {//rcv message 
            char str[64] = {0};
            int ret = read(fdr, str, sizeof(str));
            if(0 == ret)
            {
                //\TODO
                printf("chat closed.\n");
                break; 
            }
            //snd message to shm 
            snd_msg_to_shm(semid, str, RCVMSG, p);
        }
        if(FD_ISSET(STDIN_FILENO, &readset))
        {//snd message to pipe
            char str[64] = {0};
            read(STDIN_FILENO, str, sizeof(str));
            write(fdw, str, strlen(str));
            //snd message to shm 
            snd_msg_to_shm(semid, str, SNDMSG, p);
        }
    }

    //\TODO
    /* sigfunc_of_a(SIGUSR1, NULL, NULL); */
    /* shmctl(shmid, IPC_RMID, NULL); */
    /* semctl(semid, IPC_RMID, 0); */
    return 0;
}


