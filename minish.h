#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include<string.h>


#define EOL 1 /* End Of Line */
#define ARG 2 /* normal argument */
#define AMPERSAND 3
#define SEMICOLON 4
#define REDIRECTIONIN 5
#define	REDIRECTIONOUT 6

#define MAXARG 512 /* max. no. command args */
#define MAXBUF 512 /* max. length input line */
#define FOREGROUND 0
#define BACKGROUND 1
