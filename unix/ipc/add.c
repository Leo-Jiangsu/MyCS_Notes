#include <header.h>
#define N 10000000
int main(int argc, char *argv[])
{
    int shmid = shmget(1000, sizeof(int), IPC_CREAT|0666);
    int *p = (int *)shmat(shmid, NULL, 0);
    
    int semid = semget(2000, 1, IPC_CREAT|0666);
    semctl(semid, SETVAL, 0, 1);
    struct sembuf sopp = (struct sembuf){0, -1, SEM_UNDO};
    struct sembuf sopv = (struct sembuf){0, 1, SEM_UNDO};
    time_t start, end;
    time(&start);
    if(fork())
    {
        for(int i = 0; i < N; ++i)
        {
            semop(semid, &sopp, 1);
            ++*p;
            semop(semid, &sopv, 1);
        }
        wait(NULL);
        time(&end);
        printf("total time = %ld\n", end - start);
        printf("sum = %d\n", *p);
        shmdt(p);
        shmctl(shmid, IPC_RMID, NULL);
        semctl(semid, 0, IPC_RMID);
    }
    else
    {
        for(int i = 0; i < N; ++i)
        {
            semop(semid, &sopp, 1);
            ++*p;
            semop(semid, &sopv, 1);
        }
        shmdt(p);
    }
    return 0;
}

