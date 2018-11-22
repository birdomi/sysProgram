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
        switch(ch){
            case KEY_UP: printw("\rUp Arrow");
                         break;
            case KEY_DOWN: printw("\rDown Arrow");
                         break;
            default:
                printw("\rThe pressed key is %c",ch);
        }
        
    }   

    printf("\nExiting Now\n");
    endwin();

    return 0;
}
