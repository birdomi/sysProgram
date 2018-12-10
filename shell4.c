#include "smallsh.h"
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include<string.h>

static char inpbuf[MAXBUF], tokbuf[2*MAXBUF], *ptr, *tok;
static char special[] = {' ', '\t', '&', ';','>','<', '|', '\n' , '\0'};

char *prompt = "command>";

int userin(char *p){
	int c, count;

	ptr = inpbuf;
	tok = tokbuf;

	printf("%s", p);
	count =0;

	while(1){
		if((c=getchar()) ==EOF) return EOF;
		if(count <MAXBUF)
			inpbuf[count++] = c;
		if(c == '\n' && count <MAXBUF){
			inpbuf[count] = '\0';
			return count;
		}
		if(c=='\n'){
			printf(" smallsh : input line too long\n");
			count =0;
			printf("%s",p);
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
