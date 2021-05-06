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

