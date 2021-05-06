#include <header.h>
#include "func.h"
void sigfunc_register(void (*sigfunc)(int, siginfo_t *, void *))
{
    struct sigaction act;
    act.sa_flags = SA_SIGINFO;
    act.sa_sigaction = sigfunc;
    sigaction(SIGUSR1, &act, NULL);
    sigaction(SIGINT, &act, NULL);
    sigaction(SIGQUIT, &act, NULL);
}
void savepid(const char *filename)
{
    int pid = getpid();
    int fd = open(filename, O_RDWR|0666);   
    /* int ret = */ 
        write(fd, &pid, sizeof(int));
    close(fd);
}
void killpid(const char *filename)
{
    int pid = getpidfromfile(filename);
    if(pid > 0)
    {
        kill(pid, SIGUSR1);
    }
}

int getpidfromfile(const char *filename)
{
    int fd = open(filename, O_RDONLY);
    int pid = -1;
    /* int ret = */ 
        read(fd, &pid, sizeof(int));
    /* ERROR_CHECK(ret, -1, "read"); */
    /* ERROR_CHECK(ret, 0, "read"); */
    close(fd);
    return pid;
}

void snd_msg_to_msq(const char *str, int msqid, int msgtype)
{
    struct msgbuf msgp = (struct msgbuf){msgtype, {0}};
    strcpy(msgp.mtext, str);
    msgsnd(msqid, &msgp, strlen(str), 0); 
}
void print_msg_from_shm(int semid, msgbuf_t *shmp)
{
    struct sembuf sop[2];
    sop[0] = (struct sembuf){0, 1, 0};
    sop[1] = (struct sembuf){1, -1, 0};
    semop(semid, sop, 2);
    printmsg(shmp->mtype, shmp->mtext);
}

void snd_msg_to_shm(int semid, const char *msg, int msgtype, msgbuf_t *shmp)
{
    struct sembuf sop[2];
    sop[0] = (struct sembuf){0, -1, 0};
    sop[1] = (struct sembuf){1, 1, 0};
    msgbuf_t newmsg = (msgbuf_t){msgtype, {0}};            
    strcpy(newmsg.mtext, msg);
    memcpy(shmp, &newmsg, sizeof(msgbuf_t));
    semop(semid, sop, 2);
}


void print_msg_from_msq(int msqid)
{
    struct msgbuf msgp = (struct msgbuf){-1, {0}};
    msgrcv(msqid, &msgp, sizeof(msgp.mtext), 0, 0);
    printmsg(msgp.mtype, msgp.mtext);
}

void printmsg(int type, const char * msg)
{
    if(type == SNDMSG)
    {
        //output format\TODO
        printf("%s", msg);                    
    }
    else
    {
        //output format\TODO
        printf("%30s", msg);                    
    }

}
