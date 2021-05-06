#include <header.h>
//argv[1]: R
//argv[2]: W
int main(int argc, char *argv[])
{
    ARGS_CHECK(argc, 3);
    int fdr = open(argv[1], O_RDONLY);
    ERROR_CHECK(fdr, -1, "open");
    int fdw = open(argv[2], O_WRONLY);
    ERROR_CHECK(fdw, -1, "write");

    fd_set set;
    struct timeval timeout;
    while(1)
    {//wait for fdr or stdin
        FD_ZERO(&set);
        FD_SET(STDIN_FILENO, &set);
        FD_SET(fdr, &set);
        timeout.tv_sec = 2;
        timeout.tv_usec = 500000;
        int selret = select(fdw + 1, &set, NULL, NULL, &timeout);
        if(selret == 0)
        {
            printf("timeout\n");
        }
        if(FD_ISSET(STDIN_FILENO, &set))
        {
            char str[128] = {0};
            int ret = read(STDIN_FILENO, str, sizeof(str));
            if(ret == 0)
            {
                printf("close chat\n");
                break; 
            }
            write(fdw, str, strlen(str) - 1);
        }
        if(FD_ISSET(fdr, &set))
        {
            char str[128] = {0};
            int ret = read(fdr, str, sizeof(str));
            if(ret == 0)
            {
                printf("chat end\n");
                break; 
            }
            puts(str);
        }
    }
    close(fdr);
    close(fdw);
    return 0;
}

