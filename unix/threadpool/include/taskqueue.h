#ifndef __TASKQUEUE_H__
#define __TASKQUEUE_H__
#include "header.h"

typedef struct node
{
    int confd;
    //other info
    struct node *next;

}Queue_node_t, *pQueue_node_t;

typedef struct
{
    pQueue_node_t head, tail;
    int maxcapacity;
    int size;
    pthread_mutex_t mutex;
    pthread_cond_t cond;

}Task_queue_t, *pTask_queue_t;

enum 
{
    TASK_QUEUE_FULL = -3,
    TASK_QUEUE_EMPTY = -2,
    TASK_QUEUE_ERROR = -1,
    TASK_QUEUE_OK

};

int queue_node_create(pQueue_node_t *ppNew, int);
int task_queue_init(pTask_queue_t, int);
int task_dequeue(pTask_queue_t, int *);
int task_enqueue(pTask_queue_t, pQueue_node_t);
int task_queue_destroy(pTask_queue_t);
int task_queue_isempty(const pTask_queue_t);
int task_queue_isfull(const pTask_queue_t);

#endif
