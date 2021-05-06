#include <header.h>

int mod_lseek(const char* filename)
{
    int fd = open(filename, O_RDWR);
    ERROR_CHECK(fd, -1, "open");
    int ret = lseek(fd, 10240, SEEK_SET);
    ERROR_CHECK(ret, -1, "lseek");
    write(fd, "a", 1);
    close(fd);
    return 0;
}
int mod_write(const char* filename)
{
    int fd = open(filename, O_RDWR);
    ERROR_CHECK(fd, -1, "open");
    char str[10240] = {0};
    write(fd, str, 10240);
    close(fd);
    return 0;
}

int main(int argc, char *argv[])
{
    ARGS_CHECK(argc, 2);
    
    mod_lseek(argv[1]);

    /* mod_write(argv[1]); */

    return 0;
}

