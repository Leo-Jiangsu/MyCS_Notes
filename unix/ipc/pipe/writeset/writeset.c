#include <header.h>

int main(int argc, char *argv[])
{
    ARGS_CHECK(argc, 2);
    int fdr = open(argv[1], O_RDWR);
    ERROR_CHECK(fdr, -1, "open"); 
    int fdw = open(argv[1], O_RDWR);
    ERROR_CHECK(fdr, -1, "open"); 
    fd_set wset; 
    fd_set rset;
    int cnt = 0;
    while(1)
    {
        FD_ZERO(&wset);
        FD_SET(fdw, &wset);
        FD_SET(fdr, &rset);
        select(fdw + 1, &rset, &wset, NULL, NULL);
        if(FD_ISSET(fdw, &wset))
        {
            char str[1024] =  {0};
            write(fdw, str, sizeof(str));
            printf("write\n");
        }
        if(FD_ISSET(fdr, &rset))
        {
            char str[128] = {0};
            read(fdr, str, sizeof(str));
            printf("read: %d\n", cnt++);
            usleep(250000);
        }
    }
    return 0;
}

