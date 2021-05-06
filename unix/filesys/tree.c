#include <header.h>

int showdir(const char* pathname, int width)
{
    DIR *dirp = opendir(pathname);
    ERROR_CHECK(dirp, NULL, "opendir");
    struct dirent *pdirent;
    while((pdirent = readdir(dirp)) != NULL)
    {
        if(strcmp(pdirent->d_name, ".") == 0 || strcmp(pdirent->d_name, "..") == 0)
        {
            continue;     
        }
        printf("%*s%s\n", width, "", pdirent->d_name);
        if(pdirent->d_type == 4)
        {
            char path[512];
            sprintf(path, "%s/%s", pathname, pdirent->d_name);
            showdir(path, width + 4);
        }
    }

    closedir(dirp);
    return 0;
}

void printdir(const char* pathname)
{
    showdir(pathname, 0);
}


int main(int argc, char *argv[])
{
    ARGS_CHECK(argc, 2);
    printdir(argv[1]);
    return 0;
}

