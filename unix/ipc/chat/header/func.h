#ifndef _FUNC_H_
#define _FUNC_H_
#include <header.h>
#define KEY_OF_SHM 1000
#define KEY_OF_SEM 1000
#define KEY_OF_MSQ 1000
#define PIDFILENAME "srcs/pids"

typedef struct msg
{
    int mtype;//0 represent sndmsg. 1 represent rcvmsg
    char mtext[64];

}msgbuf_t;

struct msgbuf
{
    int mtype;
    char mtext[64];
};
enum
{
    SNDMSG,
    RCVMSG
};

void print_msg_from_shm(int semid, msgbuf_t *shmp);
void snd_msg_to_shm(int semid, const char *msg, int msgtype, msgbuf_t *shmp);
void snd_msg_to_msq(const char *str, int msqid, int msgtype);
void print_msg_from_msq(int msqid);
void printmsg(int type, const char * msg);
void sigfunc_register(void (*sigfunc)(int, siginfo_t *, void *));
void killpid(const char *);
void savepid(const char *filename);
int getpidfromfile(const char *filename);
/* static int pid_of_a = -1; */
/* static int pid_of_a1 = -1; */
/* static int pid_of_b = -1; */
/* static int pid_of_b1 = -1; */
#define PID_A_FILENAME "./srcs/pid_of_a"
#define PID_B_FILENAME "./srcs/pid_of_b"
#define PID_A1_FILENAME "./srcs/pid_of_a1"
#define PID_B1_FILENAME "./srcs/pid_of_b1"
#endif
