#include <header.h>
#define N 1000000

typedef struct
{
    int rsc;
    pthread_mutex_t mutex;
}info, *pinfo;



void *getrsc(void *arg)
{
    int threadrsc = 0;
    pinfo p = (pinfo)arg;
    while(1)
    {
        pthread_mutex_lock(&p->mutex);
        if(0 == p->rsc)
        {
            pthread_mutex_unlock(&p->mutex);
            printf("%ld thread get %d rsc.\n", pthread_self(), threadrsc);
            pthread_exit(0); 
        }
        else
        {
            ++threadrsc;
            --p->rsc;
        }
        pthread_mutex_unlock(&p->mutex);
    }
}


int main()
{
    info p = {N, PTHREAD_MUTEX_INITIALIZER};

    pthread_t thid1, thid2;
    
    pthread_create(&thid1, NULL, getrsc, &p);
    pthread_create(&thid2, NULL, getrsc, &p);
    pthread_join(thid1, NULL);
    pthread_join(thid2, NULL);
    return 0;
}

