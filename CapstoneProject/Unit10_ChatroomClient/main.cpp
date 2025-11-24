#include "curses.h"
#include "ChatClientUI.h"

int main() {
    // Start curses mode
    initscr();
    cbreak();       // No line buffering
    noecho();       // Don't echo typed characters
    keypad(stdscr, TRUE); // Enable arrow keys

    // Create and run the chat UI
    ChatClientUI ui;
    ui.initLayout();
    ui.run();

    // End curses mode
    endwin();
    return 0;
}
