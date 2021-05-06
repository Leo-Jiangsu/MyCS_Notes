#include <header.h>


int modi_unbuf();
int modi_buf();
int modi_mmap();


int main()
{
    /* modi_unbuf(); */
    /* modi_buf(); */
    modi_mmap();
    return 0;
}

int modi_unbuf()
{
    int fd = open("file", O_RDWR|O_TRUNC);
    ERROR_CHECK(fd, -1, "open");
    int ret = write(fd, "world", 5);
    ERROR_CHECK(ret, -1, "write");
    close(fd);
    return 0;
}

int modi_buf()
{
    FILE *fp = fopen("file", "w+");
    ERROR_CHECK(fp, NULL, "fopen");
    int ret = fwrite("world", sizeof(char), 5, fp);
    ERROR_CHECK(ret, EOF, "fwrite");
    return 0;
}


int modi_mmap()
{
    int fd = open("file",O_RDWR|O_TRUNC);
    ftruncate(fd, 5);
    char *str = (char *)mmap(NULL, 5, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);  
    ERROR_CHECK(str, (char *)-1, "mmap");
    strcpy(str, "world");
    munmap(str, 5);
    return 0;
}

