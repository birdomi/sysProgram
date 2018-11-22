#include <ncurses/curses.h>
#include <stdio.h>

int main()
{
    int ch;
    /* Curses Initialisations */
    initscr();
    raw();
    keypad(stdscr, TRUE);
    noecho();

    printf("Press E to Exit\n");

    while((ch=getch())!='e'){
        printf("\r%d pressed",ch);
    }   

    printf("\nExiting Now\n");
    endwin();

    return 0;
}
