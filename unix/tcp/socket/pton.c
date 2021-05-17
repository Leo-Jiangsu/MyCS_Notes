#include <header.h>
int main(int argc, char *argv[])
{
    ARGS_CHECK(argc, 2);
    int binip = 0;
    inet_pton(AF_INET, argv[1], &binip);
    printf("%x\n", binip);
    
    char str[32] = {0};
    inet_ntop(AF_INET, &binip, str, 32);
    printf("%s\n", str);
    return 0;
}

