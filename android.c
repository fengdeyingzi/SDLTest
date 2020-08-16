#include <stdio.h>
#include <stdlib.h>
#include "base.h"
#include <fcntl.h>

void *readFileFromAssets(char *filename, int *len){
    char temp[300];
    sprintf(temp,"assets\\%s",filename);
    *len = getlen(temp);
    void *buf = malloc(*len);
    int f = open(temp, O_RDONLY, 0777);
    read(f,buf,*len);
    close(f);
    return buf;
}

void toast(char *text, int type){
    
}
