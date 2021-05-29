#ifndef __SENDFILE_H__
#define __SENDFILE_H__

typedef struct 
{
    int paclen;
    char buf[1024];
}Filepac_t;

int send_file(int);
int sendpac(int, Filepac_t *);


#endif
