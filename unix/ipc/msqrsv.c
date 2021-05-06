#include <header.h>
struct msgbuf
{
    long mtype;
    char mtext[64];
};
int main(int argc, char *argv[])
{
    ARGS_CHECK(argc, 2);
    int msqid = msgget(1000, IPC_CREAT|0666);
    ERROR_CHECK(msqid, -1, "msgget");
    
    
    struct msgbuf msg;
    msg.mtype = atol(argv[1]);
    
    int ret = msgrcv(msqid, &msg, sizeof(msg.mtext), msg.mtype, IPC_NOWAIT);
    ERROR_CHECK(ret, -1 , "msgrcv");
    if(ret > 0)
    {
        printf("msgtype = %ld, msg = %s\n", msg.mtype, msg.mtext);
    }
    return 0;
}

