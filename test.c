#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

int main(){
    int fd, n;
    off_t start, cur;
    char buf[10];

    fd = open("vimrc", O_RDONLY);
    if(fd==-1){
        perror("open .vimrc.");
        exit(1);
    }

    while((n=read(fd,buf,10))>0){
        if(n==-1){
            perror("Read Error");
            exit(1);
        }
        printf("%s\n",buf);
    }
    close(fd);
}
