#define NDEBUG
#include <header.h>
#if 0
int redirection_stdout(const char* filename)
{
    int fd = open(filename, O_RDWR|O_CREAT, 0666);
    ERROR_CHECK(fd, -1, "open");
    int stdoutfd = 5; 
    dup2(STDOUT_FILENO, stdoutfd);//5->screen
    dup2(fd, STDOUT_FILENO);//stdout->file
    printf("stdout>file\n");
    fflush(stdout);
    dup2(stdoutfd, STDOUT_FILENO);//stdout->screen
    printf("stdout\n");
    close(fd);
    return 0;
}


int redirection_stderr(const char* filename)
{
    int fd = open(filename, O_RDWR|O_CREAT, 0666);
    ERROR_CHECK(fd, -1, "open");
    int stdoutfd = 5; 
    dup2(STDERR_FILENO, stdoutfd);//5->screen
    dup2(fd, STDERR_FILENO);//stderr->file
    fprintf(stderr,"stderr>file\n"); 
    /* fflush(stdout); */
    dup2(stdoutfd, STDERR_FILENO);//stdout->screen
    printf("stderr\n");
    close(fd);
    return 0;
}

/* inline */
int square(int x)
{
    return x * x;
}

#endif

int main(int argc, char *argv[])
{
    /* ARGS_CHECK(argc, 3); */
    /* redirection_stdout("stdout"); */
    /* redirection_stderr("stderr"); */
    /* printf("2^2 = %d\n", square(2)); */
    int a = 1;
    assert(a > 1);
    printf("ok\n");
    return 0;
}

