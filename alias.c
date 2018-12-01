#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char* argv[]){
    int fd;
    int option;
    int n;
    
    if(argc==1){
        printf("use -a to add alias\n");
        printf("use -d to delete alias\n");
    }
    else if(argc ==3){
        if(strcmp(argv[1],"-a")==0)
            option=1;
        else if(strcmp(argv[1],"-d")==0)
            option=0;
        else{
            printf("Unknown option\n");
            exit(1);
        }      
        printf("%d, size: %d\n",option,strlen(argv[2]));
        
        char *ptr=strtok(argv[2],"=");
        while(ptr!=NULL){
            printf("%s\n",ptr);
            ptr=strtok(NULL," ");
        }
        /*  
        fd = open(".alias",O_RDWR|O_APPEND|O_CREAT,0644);
        if(fd == -1){
            perror("open .alias");
            exit(1);
        }
        if(option){
            n=write(fd,argv[3],argv[3].length);
            if(n<0){
                perror("write .alias");
                exit(1);
            }
            close(fd);
            return 0;
        }
        
        else{
            
        }
        */
    }
    else{
        printf("error\n");        
        exit(1);
    }
}
