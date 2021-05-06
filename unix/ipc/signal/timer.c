#include <header.h>

time_t start;
void sigfunc(int signum, siginfo_t *info, void *p)
{
    time_t now;
    time(&now);
     
    printf("%s", ctime(&now));
}

void timertest(int signum, int timertype)
{
    struct sigaction act;
    act.sa_sigaction = sigfunc; 
    act.sa_flags = SA_SIGINFO;
    sigaction(signum, &act, NULL);
    
    time(&start);  
    printf("%s", ctime(&start));
    struct itimerval val;
    val.it_interval = (struct timeval){3, 0};
    val.it_value = (struct timeval){1, 0};
    setitimer(timertype, &val, NULL);

    sleep(3);
}

void test()
{
#if  0 
    timertest(SIGALRM, ITIMER_REAL);
#else
    timertest(SIGPROF, ITIMER_PROF);
#endif

}

int main()
{
    test();
    while(1);
    return 0;
}

