#include <header.h>

void clean_func(void *arg)
{
    pthread_mutex_t *pMutex = (pthread_mutex_t *)arg;
    pthread_mutex_unlock(pMutex);
    printf("unlock\n");
}

void *thread_func(void *arg)
{
    pthread_mutex_t *pMutex = (pthread_mutex_t *)arg;
    pthread_mutex_lock(pMutex);
    
    pthread_cleanup_push(clean_func, arg);


    printf("cancel point\n");
    pthread_mutex_unlock(pMutex);
    
    pthread_exit(0);
    pthread_cleanup_pop(1);
}

int main()
{
    pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
      

    pthread_t thid;
    int ret = pthread_create(&thid, NULL, thread_func, &mutex);
    THREAD_ERRORCHECK(ret, "pthread_create");

    pthread_cancel(thid);
    void *thread_ret;
    pthread_join(thid, &thread_ret);
    printf("thread_ret = %ld\n", (long)thread_ret);
    
    
    return 0;
}

