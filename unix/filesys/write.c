#include <header.h>

int main(int argc, char *argv[])
{
    int fd = open("StuInfo", O_RDWR|O_CREAT|O_EXCL, 0666);
    ERROR_CHECK(fd, -1, "open");
    char info[] = 
        "1001\tMary\t75\n"
        "1002\tLinda\t68\n"
        "1003\tSusan\t58\n"
        "1006\tHoward\t72\n"
        "1007\tJames\t85\n"
        "1008\tMark\t88\n"
        "1009\tDonald\t78\n";

    write(fd,info,sizeof(info)); 
    
    close(fd);
    return 0;
}

