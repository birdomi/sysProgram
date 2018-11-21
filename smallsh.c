#include "smallsh.h"

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


int main()
{
    while(userin(prompt) != EOF)
        continue;
}
