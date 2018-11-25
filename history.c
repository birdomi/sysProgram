#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include "newsh.h"
#include <ctype.h>

int main(){
    int fd,n;
    char buf[MAXBUF];

    int lineNumber=0;
    int lineSelection=0;
    int s;

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

    while(1){
        s=scanf("\nselect line to reuse:%d%d");
        if(s<lineSelection&&s>=0)
            break;  
        else{
            printf("잘못된 입력");
            exit(1);
        }
    }
    if(lineNumber>20){
        lseek(fd,20-s,SEEK_END);
    }
    else{
        lseek(fd,s,SEEK_SET);
    }
    read(fd,buf,MAXBUF);
    printf("result: %s \n",buf);

    return 0;
}
