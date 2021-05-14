#include <header.h>
int main(int argc, char *argv[])
{
    ARGS_CHECK(argc, 2);
    struct hostent *hostinfo;
    
    hostinfo = gethostbyname(argv[1]);

    printf("hostname:%s\n", hostinfo->h_name);
    for(int i = 0; hostinfo->h_aliases[i] != NULL; ++i)
    {
        printf("alias:%s\n", hostinfo->h_aliases[i]);
    }
    switch(hostinfo->h_addrtype)
    {
    case AF_INET: 
        printf("type:ipv4\n");break;
    case AF_INET6:
        printf("type:ipv6\n");break;
    }
    /* printf("type:%d\n", hostinfo->h_addrtype); */
    printf("length:%d\n", hostinfo->h_length);
    for(int i = 0; hostinfo->h_addr_list[i] != NULL; ++i)
    {
        char ipstr[64] = {0};
        inet_ntop(hostinfo->h_addrtype, hostinfo->h_addr_list[i], ipstr, 64);
        printf("ipv4:%s\n", ipstr);
    }
    
    return 0;
}


