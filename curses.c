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
    ch = getch();
    printf("%d pressed",ch);

    printf("\n\Exiting Now\n");
    endwin();

    return 0;
}
