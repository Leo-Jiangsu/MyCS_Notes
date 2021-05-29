#include "include/header.h"
#include "include/taskqueue.h"
#include "include/threadpool.h"
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
            fprintf(stderr, "%s:%d: expect:" #expect  " actual:" #actual "\n",\
                    __FILE__, __LINE__);\
        }\
    }while(0)

#define EXPECT_EQ(expect, actual) EXPECT((expect), (actual), ((expect) == (actual))); 
#define EXPECT_UNEQ(expect, actual) EXPECT((expect), (actual), ((expect) != (actual)))

void enqueue_test(pTask_queue_t pQ)
{
    pQueue_node_t pNew;
    int ret = queue_node_create(&pNew, 1);
    EXPECT_EQ(ret, TASK_QUEUE_OK);
    ret = task_enqueue(pQ, pNew);
    EXPECT_EQ(ret, TASK_QUEUE_OK);
}

void dequeue_test(pTask_queue_t pQ)
{
    int confd = -1;
    int ret = task_dequeue(pQ, &confd);
    EXPECT_EQ(ret, TASK_QUEUE_OK);
    EXPECT_UNEQ(confd, -1);
}

void task_queue_test()
{
    Task_queue_t Q;
    /* printf("") */
    int ret = task_queue_init(&Q, 3);
    EXPECT_EQ(ret, TASK_QUEUE_OK);

    ret = task_queue_isempty(&Q);
    EXPECT_EQ(ret, 1);

    enqueue_test(&Q);

    ret = task_queue_isempty(&Q);
    EXPECT_UNEQ(ret, 1);

    ret = task_queue_isfull(&Q);
    EXPECT_UNEQ(ret, 1);

    enqueue_test(&Q);
    enqueue_test(&Q);
    ret = task_queue_isfull(&Q);
    EXPECT_EQ(ret, 1);

    dequeue_test(&Q);
    dequeue_test(&Q);
    dequeue_test(&Q);

    enqueue_test(&Q);
    enqueue_test(&Q);

    ret = task_queue_destroy(&Q);
    EXPECT_EQ(ret, TASK_QUEUE_OK);
}

void thread_pool_test()
{
    Thread_pool_t tp;
    int ret = thread_pool_init(&tp, 5);
    EXPECT_EQ(ret, THREAD_POOL_OK);

    ret = thread_pool_start(&tp);
    EXPECT_EQ(ret, THREAD_POOL_OK);

    int sockfd = -1;
    ret = tcp_init("127.0.0.1", "2000", &sockfd);
    EXPECT_EQ(ret, THREAD_POOL_OK);
    EXPECT_UNEQ(sockfd, -1);
    printf("sockfd = %d\n", sockfd);

}

void test()
{
    task_queue_test();
    thread_pool_test();

    int epfd = epoll_create(1);
    EXPECT_UNEQ(epfd, -1);
    
    int ret = epoll_in_add(epfd, STDIN_FILENO);
    EXPECT_UNEQ(ret, -1);

    



}

int main()
{
    test();

    printf("(%d/%d)%.2f%% passed.\n", 
           test_pass_count, test_total_count, 
           (float)test_pass_count / test_total_count * 100);

    while(1);

    return 0;
}

