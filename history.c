#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include "newsh.h"

int main(){
    int fd,n;
    char* buf[MAXBUF];

    int lineNumber=0;
    fd = open(".history",O_RDONLY);
    if(fd == -1){
        perror("read .history");
        exit(1);
    }
    while((n=read(fd,*buf,MAXBUF))>0){
        printf("%d %s",lineNumber++,buf++);
    }

    return 0;
}
