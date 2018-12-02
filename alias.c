#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define MAX 256

int checkName(int fd,char* name){
    
}

int main(int argc, char* argv[]){
    int fd;
    int option;
    int n;
    char buf[MAX];

    if(argc==1){
        printf("use -a to add alias\n");
        printf("use -d to delete alias\n");
        printf("use -r to read alias file\n");
    }
    else if(argc ==2){
        if(strcmp(argv[1],"-r")==0){
            fd = open(".alias",O_RDONLY);
            while((n=read(fd,buf,MAX))>0){
                printf("%s",buf);
            }
            if(n<0){
                perror(".alias read error");
                exit(1);
            }
            return 0;
        }
        else{
            printf("error\n");        
            exit(1);
        }
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

        strcat(buf,o_name);
        strcat(buf,"=");
        strcat(buf,t_name);
        strcat(buf,"\n\0");

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
            n=write(fd,buf,strlen(buf));
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
