#include <header.h>

int main(int argc, char *argv[])
{
    ARGS_CHECK(argc, 2);
    struct dirent *pdirent;
    DIR *dirp= opendir(argv[1]);
    ERROR_CHECK(dirp, NULL, "opendir");
    long loc;
    while((pdirent = readdir(dirp)) != NULL)
    {
        printf("Inode = %ld,\treclen = %d, type = %d, name = %s\n"
               ,pdirent->d_ino
               ,pdirent->d_reclen
               ,pdirent->d_type
               ,pdirent->d_name
              );
        if(strcmp(pdirent->d_name, "makefile") == 0)
        {
            loc = telldir(dirp);
        }
    }
    seekdir(dirp, loc);
     
    printf("-----------\n");
    pdirent = readdir(dirp);
    ERROR_CHECK(pdirent, NULL, "readdir");

    printf("Inode = %ld,\treclen = %d, type = %d, name = %s\n"
           ,pdirent->d_ino
           ,pdirent->d_reclen
           ,pdirent->d_type
           ,pdirent->d_name
          );

    closedir(dirp);
    return 0;
}

