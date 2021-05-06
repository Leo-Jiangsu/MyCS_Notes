#include <header.h>

union semun
{
    int val;
    unsigned short *array;
}semun_t;

int main()
{
    int semid = semget(1000, 2, IPC_CREAT|0666);
    //sem[0]:product
    //sem[1]:warehouse
    semun_t.array = (unsigned short [2]){0, 10};
    semctl(semid, 0, SETALL, semun_t);

    if(fork())
    {//producer
        while(1)
        {
            struct sembuf semop_producer[2];
            semop_producer[0] = (struct sembuf){0, 1, SEM_UNDO};
            semop_producer[1] = (struct sembuf){1, -1, SEM_UNDO};
            semop(semid, semop_producer, 2);
            printf("producer:\tproduct = %d, warehouse = %d\n"
                   , semctl(semid, 0, GETVAL)
                   , semctl(semid, 1, GETVAL));
            usleep(500000);
        }
    }
    else
    {//comsumer
        while(1)
        {
            sleep(1);
            struct sembuf semop_consumer[2];
            semop_consumer[0] =  (struct sembuf){0, -1, SEM_UNDO};
            semop_consumer[1] =  (struct sembuf){1, 1, SEM_UNDO};
            semop(semid, semop_consumer, 2);
            printf("comsumer:\tproduct = %d, warehouse = %d\n"
                   , semctl(semid, 0, GETVAL)
                   , semctl(semid, 1, GETVAL));
        }
    }
    return 0;
}

