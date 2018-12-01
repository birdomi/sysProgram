#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char* argv[]){
    int fd;
    fd = open(".alias",O_RDWR|O_APPEND|O_CREAT);
    if(fd == -1){
        perror("open .alias");
        exit(1);
    }

    if(argc==1){
        printf("use -a for add alias\n");
        printf("use -d for delete alias\n");
    }
    else if(argc ==3){

    }
    else{
        printf("error\n");
        exit(1);
    }
}
