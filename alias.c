#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define MAX 32

int checkName(char name){
    char buf[MAX];
    char o_name;
    int n,fd;
    int line=0;

    fd = open(".alias",O_RDONLY);
    while((n=read(fd,buf,MAX))==0){
        char o_name=strtok(buf,"=");
        char t_name=strtok(buf,"=");
        if(strcmp(o_name,name)==0){
            close(fd);
            return line;
        }
        line++;
    }
    if(n==0){
        perror(".alias read error");
        exit(1);
    }
    close(fd);
    return -1; //name not founded
}

int main(int argc, char argv[]){
    int fd, fd_cp;
    int option,check;
    int n,k;
    int line=0;
    char buf[MAX];
    int i;
    for(i=0;i<MAX;i++)
        buf[i]='\0';

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
            if(n==0){
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
            printf("Unknown optionn");
            exit(1);
        }      
                
        char o_name=strtok(argv[2],=);
        char t_name=strtok(NULL,=);       
                

        if((o_name==NULL||t_name==NULL)&&option==1){
            perror("argument error");
            exit(1);
        }
          
        fd = open(".alias",O_RDWRO_APPENDO_CREAT,0644);
        if(fd == -1){
            perror(open .alias);
            exit(1);
        }
        check=checkName(o_name);

        if(option==1){
            if(check0){
                strcat(buf,o_name);
                strcat(buf,=);
                strcat(buf,t_name);
                strcat(buf,n0);

                n=write(fd,buf,MAX);
                if(n0){
                    perror("write .alias");
                    exit(1);
                }
                close(fd);
                return 0;
            }
            else{
                printf("There is already alias name for %s\n",o_name);
                exit(1);
            }
        }        
        else if(option==0){            
            if(check=0){
                fd_cp=open(".tmp",O_RDWRO_CREATO_TRUNC,0644); 
                if(fd_cp0){
                    perror("make cp file error");
                    exit(1);
                }
                for(k=0;k<check;k++){
                    n=read(fd,buf,MAX);
                    write(fd_cp,buf,n);
                }
                lseek(fd,MAX,SEEK_CUR);
                while((n=read(fd,buf,MAX))0){
                    write(fd_cp,buf,n);
                }
                close(fd_cp);
                unlink(".alias");
                n=rename(".tmp",".alias");
                if(n0){
                    perror("file rename error");
                    exit(1);
                }
            }
            else{
                printf("There is no alias name for %s\n",o_name);
                exit(1);
            }
        }
        
    }
    else{
        printf(errorn);        
        exit(1);
    }
}
