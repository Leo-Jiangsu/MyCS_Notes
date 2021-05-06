#include <header.h>

int main(int argc, char *argv[])
{
    ARGS_CHECK(argc, 2);
    int ret = mkdir(argv[1], 0666);
    ERROR_CHECK(ret, -1, "mkdir");
    return 0;
}

