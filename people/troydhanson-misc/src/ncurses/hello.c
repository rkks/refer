#include <ncurses.h>

int main() {
  initscr();
  printw("Hello world!");  // writes to offscreen buffer
  refresh();               // update the real window
  getch();                 // wait for keystroke
  endwin();                // restore window 
  return 0;
}
