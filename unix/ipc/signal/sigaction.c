#include <header.h>

#if 0
void handler(int signum, siginfo_t *info, void *ucontext)
{
    printf("SIGINT: %d\n", signum);
    sleep(3);
    sigset_t set;
    sigemptyset(&set);
    sigpending(&set);
    if(sigismember(&set, SIGQUIT))
    {
        printf("SIGQUIT is pengding\n"); 
    }
    else
    {
        printf("SIGQUIT is not pengding\n"); 
    }
    printf("SIGINT end\n");
}

#endif
int main()
{
    /* struct sigaction act; */
    /* act.sa_sigaction = handler; */
    /* act.sa_flags = SA_SIGINFO; */
    /* sigset_t set; */
    /* sigemptyset(&set); */
    /* sigaddset(&set, SIGQUIT); */
    /* act.sa_mask = set; */

    /* sigaction(SIGINT, &act, NULL); */
    
    sigset_t set;
    sigemptyset(&set);
    /* sigaddset(&set, SIGINT); */
    sigfillset(&set);
    sigprocmask(SIG_BLOCK, &set, NULL);

    //key code
    printf("key code\n");
    sleep(5);
    printf("key code end\n");
    //key code end
    
    sigprocmask(SIG_UNBLOCK, &set, NULL);

    while(1);
    return 0;
}

