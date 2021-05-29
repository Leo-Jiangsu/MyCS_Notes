#include "../include/taskqueue.h"
int queue_node_create(pQueue_node_t *ppNew, int confd)
{
    *ppNew = (pQueue_node_t)malloc(sizeof(Queue_node_t));
    if(!*ppNew)
    {   
        return TASK_QUEUE_ERROR;
    }   
    (*ppNew)->confd= confd;
    (*ppNew)->next = NULL;
    return TASK_QUEUE_OK;

}

int task_queue_init(pTask_queue_t pQ, int maxcapacity)
{
    if(!pQ)
    {
        return TASK_QUEUE_ERROR;
    }
    memset(pQ, 0 , sizeof(Task_queue_t));
    pQ->head = pQ->tail = NULL;
    pQ->maxcapacity = maxcapacity;
    pQ->size = 0;
    pthread_mutex_init(&pQ->mutex, NULL);
    /* (*pQ->mutex = PTHREAD_MUTEX_INITIALIZER; */
    pthread_cond_init(&pQ->cond, NULL);
    /* (*pQ->cond = PTHREAD_COND_INITIALIZER; */
    return TASK_QUEUE_OK;
}

int task_dequeue(pTask_queue_t pQ, int *confd)
{
    if(task_queue_isempty(pQ))
    {
        return TASK_QUEUE_EMPTY;
    }
    *confd = pQ->head->confd;
    pQueue_node_t pTemp = pQ->head;
    pQ->head = pQ->head->next;
    --pQ->size;
    if(task_queue_isempty(pQ))
    {
        pQ->tail = pQ->head = NULL;
    }
    free(pTemp);
    return TASK_QUEUE_OK;
}

int task_enqueue(pTask_queue_t pQ, pQueue_node_t pQNode)
{
    if(task_queue_isfull(pQ))
    {
        return TASK_QUEUE_FULL;
    }
    if(task_queue_isempty(pQ))
    {
        pQ->head = pQ->tail = pQNode;
    }
    else
    {
        pQ->tail->next = pQNode;
        pQ->tail = pQNode;
    }
    ++pQ->size;
    return TASK_QUEUE_OK;
}

int task_queue_destroy(pTask_queue_t pQ)
{
    if(!task_queue_isempty(pQ))
    {
        while(pQ->head)
        {
            pQueue_node_t pTemp = pQ->head;
            pQ->head = pTemp->next;
            free(pTemp);
        }
        return TASK_QUEUE_OK;
    }
    return TASK_QUEUE_EMPTY;
}

int task_queue_isempty(const pTask_queue_t pQ)
{
    return pQ->size == 0;
}

int task_queue_isfull(const pTask_queue_t pQ)
{
    return pQ->size == pQ->maxcapacity;
}

