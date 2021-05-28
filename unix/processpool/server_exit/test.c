#include "include/process_pool.h"

static int test_total_count = 0;
static int test_pass_count = 0;

#define EXPECT(expect, actual, equality) \
    do{\
        ++test_total_count;\
        if((equality))\
        {\
            ++test_pass_count;\
        }\
        else{\
            fprintf(stderr, "%s:%d: expect: #expect  actual: #actual\n",\
                    __FILE__, __LINE__);\
        }\
    }while(0)

#define EXPECT_EQ(expect, actual) EXPECT(expect, actual, ((expect) == (actual))); 
#define EXPECT_UNEQ(expect, actual) EXPECT(expect, actual, ((expect) != (actual)))

void test_create_child_process(int childProcessCount)
{
    process_data_t *childArr = 
        (process_data_t *)calloc(childProcessCount, sizeof(process_data_t));
    
    int ret = create_child_process(childArr, childProcessCount);
    EXPECT_UNEQ(ret, -1);
}
void test_parent_listen_socket(char *addr, char *port)
{
    int sockfd= parent_listen_socket(addr, port);
    EXPECT_UNEQ(sockfd, -1);
}



void test(int argc, char*argv[])
{
    EXPECT_EQ(argc, 4);
    /* test_create_child_process(atoi(argv[3])); */
    /* test_parent_listen_socket(argv[1], argv[2]); */
}

int exitpipe[2] = {-1, -1};
void sigfunc(int arg)
{
    int exitflag = 1;
    write(exitpipe[1], (int*)&exitflag, 1);
    printf("sig arg = %d\n", arg);
}



int main(int argc, char *argv[])
{
    ARGS_CHECK(argc, 4);
#if 1
    //Init child process
    int childProcessCount = atoi(argv[3]);
    process_data_t *childArr = 
        (process_data_t *)calloc(childProcessCount, sizeof(process_data_t));
    int ret = create_child_process(childArr, childProcessCount);
    ERROR_CHECK(ret, -1, "create_child_process");

#endif
#ifdef SERVEREXIT
    pipe(exitpipe);
    signal(SIGUSR1, sigfunc);


#endif

#if 1
    //create socket and listen 
    int sockfd= parent_listen_socket(argv[1], argv[2]);
    ERROR_CHECK(sockfd, -1, "parent_listen_socket");
#endif

#if 1
    //epoll socketfd and childfd
    ret = parent_epoll(sockfd, childArr, childProcessCount);
    ERROR_CHECK(ret, -1, "parent_epoll");
#endif

#if 1
    //

#endif

#if 1
    //DEBUG
    test(argc, argv);
    printf("(%d/%d)%.2f%% passed.\n", 
           test_pass_count, test_total_count, 
           (float)test_pass_count / test_total_count * 100);

#endif

    while(1);
    return 0;
}

