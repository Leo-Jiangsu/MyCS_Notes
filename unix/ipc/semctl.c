#include <header.h>
union semun
{
    int val;
    unsigned short *array;
}semun_t;

void setval(int semid, int semnum);
int getval(int semid, int semnum);

void getall(int semid);
void setall(int semid);
int main(int argc, char *argv[])
{
    int semid = semget(1000, 3, IPC_CREAT|0666);
    ERROR_CHECK(semid, -1, "semget");
    setval(semid, 1);
    printf("sem = %d\n", getval(semid, 1));
    setall(semid);
    getall(semid);
    printf("sems = [%d %d %d]\n"
           , semun_t.array[0], semun_t.array[1], semun_t.array[2]); 
    
    setval(semid, 1);
    printf("sem = %d\n", getval(semid, 1));
    semctl(semid, 0, IPC_RMID);
    return 0;
}
void setval(int semid, int semnum)
{
    semun_t.val = 5;
    semctl(semid, semnum, SETVAL, semun_t);
}
int getval(int semid, int semnum)
{
    return semctl(semid, semnum, GETVAL);    
}
void setall(int semid)
{
    semun_t.array = (unsigned short[3]){1, 2, 5};
    semctl(semid, 0, SETALL, semun_t);
}
void getall(int semid)
{
    semctl(semid, 0, GETALL, semun_t);
}
