#include <header.h>

int main(int argc, char *argv[])
{
#if 1
    DIR *dirp = opendir(getcwd(NULL, 0));
    ERROR_CHECK(dirp, NULL, "opendir");
    struct dirent *pdirent;
    while((pdirent = readdir(dirp)) != NULL)
    {
        struct stat statbuf;
        /* char *pathname = (char *)malloc(strlen(getcwd(NULL, 0) + strlen(pdirent->d_name) + 2)); */
        /* sprintf(pathname, "%s/%s", getcwd(NULL, 0), pdirent->d_name); */
        /* printf("%s\n",pathname); */

        int ret = stat(pdirent->d_name, &statbuf);
        ERROR_CHECK(ret, -1, "stat");
        char modiftime[128] = {0};
        strcpy(modiftime, ctime(&statbuf.st_mtime));
        modiftime[strlen(modiftime) - 1] = '\0';
        printf("%o\t%ld %s %s %ld\t%s %s\n"
               ,statbuf.st_mode
               ,statbuf.st_nlink
               ,getpwuid(statbuf.st_uid)->pw_name
               ,getgrgid(statbuf.st_gid)->gr_name
               ,statbuf.st_size
               ,modiftime
               ,pdirent->d_name 
              );

        /* free(pathname); */
    
    }
    closedir(dirp);
#else
    ARGS_CHECK(argc, 2);
    struct stat statbuf;
    int ret = stat(argv[1], &statbuf);
    ERROR_CHECK(ret, -1, "stat");
    printf("%o %ld %s %s %ld %s"
           ,statbuf.st_mode
           ,statbuf.st_nlink
           ,getpwuid(statbuf.st_uid)->pw_name
           ,getgrgid(statbuf.st_gid)->gr_name
           ,statbuf.st_size
           ,ctime(&statbuf.st_mtime)
          );
#endif
    return 0;
}

