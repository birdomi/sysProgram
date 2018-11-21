#include "smallsh.h"
#include <stdlib.h>

static char inpbuf[MAXBUF], tokbuf[2*MAXBUF], *ptr, *tok;
static char special[] = {' ', '\t', '&', ';', '\n', '\0'};

char *prompt = "Command>";

int userin(char *p){
    int c, count;

    ptr = inpbuf;
    tok = tokbuf;

    printf("%s ", p);
    count = 0;

    while(1){
        if((c=getchar())==EOF) return EOF;
        if(count <MAXBUF) inpbuf[count++] = c;
        if(c=='\n' && count <MAXBUF){
            inpbuf[count] = '\0';
            return count;
        }        
        if(c=='\n'){
            printf(" smallsh : input line too long\n");
            count = 0;
            printf("%s ", p);
        }
    }    
}

//get token and place into tokbuf
int get_token(char **outptr){
    int type;

    *outptr = tok;
    while(*ptr == ' '||*ptr == '\t')
        ptr++;
        
    *tok++= *ptr;

    switch(*ptr++) {
        case '\n' : type = EOL;
            /* printf(" type == EOL getok()\n"); */
            break;
        case '&' : type = AMPERSAND;
            /* printf(" type == AMPERSAND getok()\n"); */
            break;
        case ';' : type = SEMICOLON;
            /* printf(" type == SEMICOLON getok()\n"); */
            break;
        default : type = ARG;
            /* printf(" type == ARG getok()\n"); */
            while(inarg(*ptr))
                *tok++ = *ptr++;
    }
    *tok++ = '\0';
    return type;
}

int inarg(char c){
    char *wrk;
    for(wrk = special; *wrk != '\0'; wrk++) {
        if (c == *wrk) {
            printf(" special arg : %c inarg()\n", *wrk);
            return(0);
        }
    }
    return(1);
}

/* 입력 줄을 아래와 같이 처리한다 : */
/* */
/* gettok을 이용하여 명령문을 구무분석(parse) 하고 */
/* 그 과정에서 인수의 목록을 작성한다. 개행문자나 */
/* 세미콜론(;)을 만나면 명령을 수행하기 위해 */
/* runcommand라 불리는 루틴을 호출한다. */
void procline()
{
    char *arg[MAXARG+1]; /* runcommand를 위한 포인터 배열 */
    int toktype; /* 명령내의 토근의 유형 */
    int narg; /* 지금까지의 인수 수 */
    int type; /* FOREGROUND or BACKGROUND */
    /* 토큰 유형에 따라 행동을 취한다. */
    for (narg = 0;;) { /* loop FOREVER */
        switch(toktype = get_token(&arg[narg])) {
            case ARG : 
                if (narg < MAXARG) narg++;
                break;
            case EOL :
            case SEMICOLON :
            case AMPERSAND :
                type = (toktype == AMPERSAND) ?
                BACKGROUND : FOREGROUND;
        if (narg != 0) {
            arg[narg] = NULL;
            runcommand(arg, type);
        }
        if (toktype == EOL) return;
        narg = 0;
        break;
        }
    }
}

/* wait를 선택사항으로 하여 명령을 수행한다. */
/* 명령을 수행하기 위한 모든 프로세스를 시작하게 한다. */
/* 만일 where가 smallsh.j에서 정의된 값 BACKGROUND로 */
/* 지정되어 있으면 waitpid 호출은 생략되고 runcommand는 */
/* 단순히 프로세스 식별번호만 인쇄하고 복귀한다. */
int runcommand(char **cline, int where)
{
    int status;
    int pid, exitstat, ret;
    if ((pid = fork()) < 0) {
        perror("smallsh");
        return(-1);
    }
    if (pid == 0) { /* child */
        execvp(*cline, cline);
        perror(*cline);
        exit(127);
    }
    /* code for parent */
    /* if background process, print pid and exit */
    if (where == BACKGROUND) {
        printf("[Process id %d]\n",pid);
        return(0);
    }
    /* 프로세스 pid가 퇴장할 때까지 기다린다. */
    if (waitpid(pid, &status, 0) == -1)
        return (-1);
    else
        return (status);
}
int main()
{
    while(userin(prompt) != EOF)
        procline();
}
