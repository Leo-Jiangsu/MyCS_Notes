#include <header.h>
#if 1
struct msgbuf
{
    long mtype;
    char mtext[64];
};
#endif
int main(int argc, char *argv[])
{
    ARGS_CHECK(argc, 3);
    int msqid = msgget(1000, IPC_CREAT|0666);
    ERROR_CHECK(msqid, -1, "msgget");
    
    
    struct msgbuf msg;
    msg.mtype = atol(argv[1]);
    strcpy(msg.mtext, argv[2]);

    msgsnd(msqid, &msg, strlen(msg.mtext), 0);

    return 0;
}

