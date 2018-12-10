#include "minish.h"

static char inpbuf[MAXBUF], tokbuf[2*MAXBUF], *ptr, *tok;
static char special[] = {' ', '\t', '&', ';','>','<', '|', '\n' , '\0'};

char *prompt = "command>";

int checkName(char* inputBuf){
    char buf[32];
    char input[MAXBUF];
    char result[MAXBUF]="";
    char* token;
    
    int n,fd,pos;
    int check=0;
    strcpy(input,inputBuf);
    fd = open(".alias",O_RDONLY);
    if(fd<0){
        return -1;
    }
    token = strtok(input," \n");
    if(token==NULL)
        return -1;
    while((n=read(fd,buf,32))>0){
        char* o_name=buf;
        char* t_name=strchr(buf,'=');  
        buf[(int)t_name-(int)o_name]='\0';
        t_name++;        
        
        if(strcmp(token,o_name)==0){
            check=1;            
            strcat(result,t_name);
            strcat(result," ");
        }
    }
    if(check==0){
        strcat(result,token);
        strcat(result," ");
    }
    check=0;
    lseek(fd,0,SEEK_SET);

    while(token=strtok(NULL," \n")){
        while((n=read(fd,buf,32))>0){
            char* o_name=buf;
            char* t_name=strchr(buf,'=');  
            buf[(int)t_name-(int)o_name]='\0';
            t_name++;
            
            if(strcmp(token,o_name)==0){
                check=1;
                strcat(result,t_name);
                strcat(result," ");
            }
        }
        if(check==0){
            strcat(result,token);
            strcat(result," ");
        }
        check=0;
        lseek(fd,0,SEEK_SET);
    }
    
    if(n<0){
        perror(".alias read error");
        exit(1);
    }
    close(fd);  
    strcat(result,"\n");
    strcpy(inputBuf,result);
    return 0;
}

int readHistory(char* readBuf){
    int fd,n;
    char buf[MAXBUF];

    int lineNumber=0;
    int lineSelection=0;
    int s;

    fd = open(".history",O_RDONLY);
    if(fd == -1){
        printf("\nThere is no history.");
        return 0;
    }
    while((n=read(fd,buf,MAXBUF))>0){
        lineNumber++;
    }
    
    if(lineNumber>20){
        lseek(fd,-20*MAXBUF,SEEK_END);
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
        printf("select line(-1 to exit): ");
        
        scanf("%d",&s);        
        if(s<lineSelection&&s>=0){
            getchar();
            break;
        }
        else{
            if(s==-1){   
                for(n=0;n<MAXBUF;n++)
                    readBuf[n]='\0';             
                close(fd);
                return 0;
            }
            printf("Wrong Input\n");    
            while (getchar() != '\n');        
        }
    }
    if(lineNumber>20){
        lseek(fd,-20*MAXBUF+s*MAXBUF,SEEK_END);
    }
    else{
        lseek(fd,s*MAXBUF,SEEK_SET);
    }
    read(fd,readBuf,MAXBUF);
    close(fd);
    return 1;
}

int userin(char *p){
	int c, count;
    
	ptr = inpbuf;
	tok = tokbuf;

	printf("%s", p);
	count =0;

	while(1){
		c=getchar();
        if(c==EOF) return EOF;
        if(count <MAXBUF) inpbuf[count++] = c;
        if(strcmp(inpbuf,"history")==0){
                if(readHistory(inpbuf)){
                    return 1;
                }
                else{
                    count = 0;
                    continue;
                }
            }
        if(c=='\n' && count <MAXBUF){           
            inpbuf[count] = '\0';    
            checkName(inpbuf);
            return count;
        }        
        if(c=='\n' && count >= MAXBUF){
            printf(" smallsh : input line too long\n");
            count = 0;
            printf("%s ", p);
        }
	}
}

int inarg(char c){
	char *wrk;
	for(wrk = special; *wrk != '\0';wrk++){
		if(c == *wrk){
			//printf("special arg : %c inarg()\n", *wrk);
			return(0);
		}	
	}
	return(1);
}


 void redirectionin(char **cline){
	
	int fd;
	char *t;
	
	for(;*cline != "\0";){
		if(*cline == 0x0 ){
			*cline--;
			break;
		}
			*cline++;
	}

	t= *cline;	
	
		
	fd=open(t, O_RDONLY, 0644);
	if(fd == -1){
		perror("open");
		exit(1);
	}
	dup2(fd,0);
	close(fd);
		
}
void redirectionout(char **cline){
	int fd;
	char * t;
	for(;*cline != "\0";){
		if(*cline == 0x0 ){
			*cline--;
			break;
		}
			*cline++;
	}

	t= *cline;	

		
	fd=open(t, O_CREAT | O_RDWR | O_TRUNC , 0644);
	if(fd == -1){
		perror("open");	
	}
	dup2(fd,1);

	close(fd);
}

char **filearg(char **cline){
	if(strcmp(*cline,"ls")==0 || strcmp(*cline,"ps") ==0)  {
			for(; *cline != "\0";){
				if(*cline == 0x0){
					*cline--;
					break;
				}
				*cline++;
			}
		*cline = 0x0;
		*cline--;
		 while(1){
			if(strcmp(*cline,"ls")==0 || strcmp(*cline,"ps")==0)
				break;
			*cline--;
		}
		
	}
	return cline;
}

int get_token(char **outptr){
	int type;

	*outptr = tok;
	while(*ptr == ' ' || *ptr == '\t')
		ptr++;

	*tok++ = *ptr;
	
	switch(*ptr++){
		case '\n': type = EOL;
		
			break;
		case '&' : type = AMPERSAND;
			
			break;
		case ';' : type = SEMICOLON;
		
			break;
		case '<' : type = REDIRECTIONIN;
			break;
		case '>' : type = REDIRECTIONOUT;
			break;
		default : type = ARG;
		
			while(inarg(*ptr))
				*tok++ = *ptr++;
	}
	*tok++ = '\0';
//	printf("outToken:%s", *outptr);
	return type;
}
void procline()
{
	char *arg[MAXARG+1];
	int toktype;
	int narg;
	int type;
	int redirec;
	for(narg = 0 ;;){
		switch(toktype = get_token(&arg[narg])){
			case ARG :
				if(narg < MAXARG) narg++;
				break;
			case REDIRECTIONIN :
				redirec = REDIRECTIONIN;
				break;
			case REDIRECTIONOUT: 
				redirec = REDIRECTIONOUT;
				break;
			case EOL:
			case SEMICOLON :
			case AMPERSAND :
				type = (toktype == AMPERSAND) ? BACKGROUND : FOREGROUND;
				if(narg!=0){
					arg[narg] = NULL;
					runcommand(arg,type,redirec);
				}
				if(toktype == EOL) return;
				narg = 0;
				break;
				}
	}
}

int runcommand(char **cline, int where, int redirec ){
	int status;
	int pid, exitstat,ret;
	//printf("%s\n", *cline);	

    int fd, n;
    fd = open(".history",O_RDWR|O_CREAT|O_APPEND,0644);
    if(fd==-1){
        perror("open .history");
        exit(1);
    }    
    if(write(fd,inpbuf,MAXBUF)<0){
        perror("write .history");
        exit(1);
    }
    close(fd);
    //***

	if((pid = fork()) <0) {
		perror("smallsh");
		return(-1);
	}
	if(pid ==0){
		if(redirec == REDIRECTIONIN){
			redirectionin(cline);
		}
		if(redirec == REDIRECTIONOUT){
			redirectionout(cline);
			cline = filearg(cline);
		}
		execvp(*cline,cline);
		perror(*cline);
		exit(127);
	}
	if(where == BACKGROUND){
		printf("[Process id %d]\n",pid);
		return (0);
	}
	if(waitpid(pid,&status,0) == -1)
		return (-1);
	else
		return (status);

}



int main()
{
	while(userin(prompt) != EOF){
		procline();
	}
}
