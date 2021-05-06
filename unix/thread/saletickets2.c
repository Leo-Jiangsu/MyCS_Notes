#include <header.h>

typedef struct
{
    int ticket_cnt;
    pthread_mutex_t mutex;
    pthread_cond_t cond;
}ticket_info, *pticket_info;

void *salewindow1(void *arg)
{
    pticket_info p = (pticket_info)arg; 
    while(1)
    {
        pthread_mutex_lock(&p->mutex);
        if(0 == p->ticket_cnt)
        {//no tickets, signal to set
            printf("no tickets\n");
            pthread_cond_wait(&p->cond, &p->mutex); 
        }
        else
        {//else sale ticket
            --p->ticket_cnt;
            printf("window1: sale ticket\n"); 
        }
        pthread_mutex_unlock(&p->mutex);    
        sleep(1); 
    }

}
void *salewindow2(void *arg)
{
    pticket_info p = (pticket_info)arg; 
    while(1)
    {
        pthread_mutex_lock(&p->mutex);
        if(0 == p->ticket_cnt)
        {//no tickets, signal to set
            printf("no tickets\n");
            pthread_cond_wait(&p->cond, &p->mutex); 
        }
        else
        {//else sale ticket
            --p->ticket_cnt;
            printf("window2: sale ticket\n"); 
        }
        pthread_mutex_unlock(&p->mutex);    
        sleep(1); 
    }

}

void *setticket(void *arg)
{
    pticket_info p = (pticket_info)arg;
    while(1)
    {
        pthread_mutex_lock(&p->mutex);
        if(0 == p->ticket_cnt)
        {//set tickets
            p->ticket_cnt = 10;
            printf("set tickets\n");
            pthread_cond_broadcast(&p->cond);
        }
        else
        {//wait

        }
        pthread_mutex_unlock(&p->mutex);   
    }
}


int main()
{
    ticket_info p = {10, PTHREAD_MUTEX_INITIALIZER, PTHREAD_COND_INITIALIZER};
    /* p.mutex = ; */
    /* p.cond = ; */
    /* p.ticket_cnt = 10; */

    pthread_t thid_sale1, thid_sale2, thid_set;
    pthread_create(&thid_sale1, NULL, salewindow1, &p);
    pthread_create(&thid_sale2, NULL, salewindow2, &p);
    pthread_create(&thid_set, NULL, setticket, &p);
    
    pthread_join(thid_sale1, NULL); 
    pthread_join(thid_sale2, NULL); 
    pthread_join(thid_set, NULL);

    return 0;
}

