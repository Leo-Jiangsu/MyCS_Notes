#include <header.h>

void handler(int signum)
{
    printf("SIGINT: %d\n", signum);
}

int main()
{
    signal(SIGINT, handler);
    sleep(10);
    signal(SIGINT, SIG_DFL);
    
    while(1);
    return 0;
}

