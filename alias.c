#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char* argv[]){
    int fd;
    int option;
    int n;
    
    if(argc==1){
        printf("use -a for add alias\n");
        printf("use -d for delete alias\n");
    }
    else if(argc ==3){
        if(argv[2]=="-a")
            option=1;
        else if(argv[2]=="-d")
            option=0;
        else{
            printf("Unknown option\n");
            exit(1);
        }        
        fd = open(".alias",O_RDWR|O_APPEND|O_CREAT,0644);
        if(fd == -1){
            perror("open .alias");
            exit(1);
        }
        if(option){
            n=write(".alias",argv[3],arg[3].length);
            if(n<0){
                perror("write .alias");
                exit(1);
            }
            close(fd);
            return 0;
        }
        else{
            
        }
    }
    else{
        printf("error\n");        
        exit(1);
    }
}
