#include <header.h>

int main(int argc, char *argv[])
{
    ARGS_CHECK(argc, 2);
    int fdr = open(argv[1], O_RDONLY);
    ERROR_CHECK(fdr, -1, "open");
    char str[128] = {0};
    read(fdr, str, sizeof(str));
    puts(str);
    close(fdr);
    return 0;
}

