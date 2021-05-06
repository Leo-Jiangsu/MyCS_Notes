#include <header.h>
//argv[1]: W
//argv[2]: R
int main(int argc, char *argv[])
{
    ARGS_CHECK(argc, 3);
    int fdw = open(argv[1], O_WRONLY);
    ERROR_CHECK(fdw, -1, "write");
    int fdr = open(argv[2], O_RDONLY);
    ERROR_CHECK(fdr, -1, "open");
    
    fd_set set;
    while(1)
    {//wait for fdr or stdin
        FD_ZERO(&set);
        FD_SET(STDIN_FILENO, &set);
        FD_SET(fdr, &set);
        select(fdr + 1, &set, NULL, NULL, NULL);
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

