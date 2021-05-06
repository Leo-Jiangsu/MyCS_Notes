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
    
    char str[128];
    while(1)
    {
        memset(str, 0, sizeof(str));
        read(STDIN_FILENO, str, sizeof(str));
        write(fdw, str, strlen(str) - 1);
        memset(str, 0, sizeof(str));
        read(fdr, str, sizeof(str));
        puts(str); 
    }
    
    close(fdr);
    close(fdw);
    return 0;
}

