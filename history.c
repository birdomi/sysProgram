#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include "newsh.h"

int main(){
    int fd,n;
    char buf[MAXBUF];

    int lineNumber=0;
    int lineSelection=0;

    fd = open(".history",O_RDONLY);
    if(fd == -1){
        perror("read .history");
        exit(1);
    }
    while((n=read(fd,buf,MAXBUF))>0){
        lineNumber++;
    }
    
    if(lineNumber>20){
        lseek(fd,-20,SEEK_END);
        while((n=read(fd,buf,MAXBUF))>0){
            printf("%d %s",lineSelection,buf);
            lineSelection++;
        }
    }
    else{
        lseek(fd,0,SEEK_SET);
        while((n=read(fd,buf,MAXBUF))>0){
            printf("%d %s",lineSelection,buf);
            lineSelection++;
        }
    }

    return 0;
}
