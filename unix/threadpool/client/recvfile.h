#ifndef __RECVFILE_H__
#define __RECVFILE_H__
#include "header.h"

typedef struct
{
    int paclen;
    char buf[1024];
}Filepac_t;

/* #define FILENAME "file" */

/* int recvbuf(int, void *buf, size_t len); */
int recvpac(int, Filepac_t *);
int recvfile(int);

#endif
