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
        
        char *o_name=strtok(argv[2],"=");
        char *t_name=strtok(NULL," ");
        if(o_name==NULL||t_name==NULL){
            perror("argument error");
            exit(1);
        }

        printf("%s %s\n",o_name,t_name);
          
        fd = open(".alias",O_RDWR|O_APPEND|O_CREAT,0644);
        if(fd == -1){
            perror("open .alias");
            exit(1);
        }
        if(option){
            n=write(fd,argv[2],strlen(argv[2]));
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
