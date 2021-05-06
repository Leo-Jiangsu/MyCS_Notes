#include <header.h>

typedef struct stuinfo
{
    int node;
    char name[20];
    int mark;
    struct stuinfo *next; 
}stuinfo;

void destoryList(stuinfo *List);

int readInfo(const char *filename, stuinfo *List);

void sortList(stuinfo *List);


int main()
{
    stuinfo *List = (stuinfo *)malloc(sizeof(stuinfo));
    readInfo("StuInfo", List);
    sortList(List);
    destoryList(List);
    return 0;
}

void destoryList(stuinfo *List)
{
    stuinfo *pcur = List;
    while(pcur)
    {
        stuinfo *temp = pcur;
        pcur = pcur->next;
        free(temp);
    }
}


int readInfo(const char *filename, stuinfo *List)
{
    int fd = open(filename, O_RDONLY);
    ERROR_CHECK(fd, -1, "open");
    
    char str[128] = {0};
    char ch;
    int index = 0;
    stuinfo *pcur = List;
    while(read(fd, &ch, sizeof(char)) == 1)
    {
        if(ch != '\n')
        {
            str[index++] = ch;
        }
        else
        {
            stuinfo *pnew = (stuinfo *)malloc(sizeof(stuinfo));
            sscanf(str, "%d\t%s\t%d", &pnew->node, pnew->name, &pnew->mark); 
            pcur->next = pnew;
            pcur = pnew;
            memset(str, 0, sizeof(str));
            index = 0;
        }
    }
    close(fd);
    return 0;
}


void sortList(stuinfo *List)
{
    stuinfo *pcur = List->next;
    int size = 0;
    while(pcur)
    {
        ++size;
        pcur = pcur->next;
    }
    stuinfo **listarr = (stuinfo **)malloc(size * sizeof(stuinfo *));
    /* int *markarr = (int *)malloc(size * sizeof(int)); */
    pcur = List->next;
    int idx = 0; 
    while(pcur)
    {
        listarr[idx++] = pcur;
        pcur = pcur->next;
    }
    for(int i = 0; i < size - 1; ++i)
    {
        for(int j = i + 1; j < size; ++j)
        {
            if(listarr[j]->mark > listarr[i]->mark)
            {
                stuinfo *temp = listarr[j];
                listarr[j] = listarr[i];
                listarr[i] = temp;
            }
        }
    }
    for(int i = 0; i < size; ++i)
    {
        printf("%d\t%s\t%d\n", listarr[i]->node, listarr[i]->name, listarr[i]->mark);
    }
    
    free(listarr);

}

