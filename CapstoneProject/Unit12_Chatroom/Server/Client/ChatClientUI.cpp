#include "ChatClientUI.h"
#include <algorithm>
#include <cctype>


// Constructor: sets up curses UI
ChatClientUI::ChatClientUI()
{
    // Initialize curses mode
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    start_color();

    // Color pairs used in the UI
    init_pair(1, COLOR_YELLOW, COLOR_BLACK);   // messages
    init_pair(2, COLOR_CYAN, COLOR_BLACK);     // rooms
    init_pair(3, COLOR_WHITE, COLOR_BLACK);    // header
    init_pair(4, COLOR_GREEN, COLOR_BLACK);    // input
    init_pair(5, COLOR_MAGENTA, COLOR_BLACK);  // selected room

    // Get terminal size
    getmaxyx(stdscr, maxY, maxX);

    // Calculate window widths
    roomWidth = maxX / 4;
    messageWidth = maxX - roomWidth - 3;

    // Create windows
    winHeader = newwin(3, maxX, 0, 0);
    winMessages = newwin(maxY - 6, messageWidth, 3, 0);
    winRooms = newwin(maxY - 6, roomWidth, 3, messageWidth + 2);
    winInput = newwin(3, maxX, maxY - 3, 0);

    // Draw initial UI
    drawHeader();
    drawRooms();
    drawMessages();
    drawInput();
}


// Destructor: clean up curses
ChatClientUI::~ChatClientUI()
{
    delwin(winHeader);
    delwin(winMessages);
    delwin(winRooms);
    delwin(winInput);
    endwin();
}



// Add a message to the message list
void ChatClientUI::addMessage(const string& msg)
{
    // Store message (single-room view for now)
    roomMessages[""].push_back(msg);

    // Redraw messages window
    drawMessages();
}


// Refresh entire UI
void ChatClientUI::refreshAll()
{
    drawHeader();
    drawRooms();
    drawMessages();
    drawInput();
}



// Draw the top header bar
void ChatClientUI::drawHeader()
{
    werase(winHeader);
    box(winHeader, 0, 0);

    wattron(winHeader, COLOR_PAIR(3));
    mvwprintw(winHeader, 1, 2, "CSC-284 Chat Client");
    wattroff(winHeader, COLOR_PAIR(3));

    wrefresh(winHeader);
}



// Draw the rooms panel
void ChatClientUI::drawRooms()
{
    werase(winRooms);
    box(winRooms, 0, 0);

    wattron(winRooms, COLOR_PAIR(2));
    mvwprintw(winRooms, 1, 2, "Rooms");
    wattroff(winRooms, COLOR_PAIR(2));

    wrefresh(winRooms);
}



// Draw chat messages window
void ChatClientUI::drawMessages()
{
    werase(winMessages);
    box(winMessages, 0, 0);

    wattron(winMessages, COLOR_PAIR(1));

    int line = 1;
    for (const string& msg : roomMessages[""])
    {
        // Stop if we run out of vertical space
        if (line >= maxY - 8)
            break;

        mvwprintw(winMessages, line++, 2, "%s", msg.c_str());
    }

    wattroff(winMessages, COLOR_PAIR(1));
    wrefresh(winMessages);
}



// Draw input box
void ChatClientUI::drawInput()
{
    werase(winInput);
    box(winInput, 0, 0);
    mvwprintw(winInput, 1, 2, "> ");
    wrefresh(winInput);
}



// Get user input from input window
string ChatClientUI::getInput()
{
    drawInput();

    string buffer;
    int ch;

    while (true)
    {
        ch = wgetch(winInput);

        // Enter key submits input
        if (ch == '\n' || ch == '\r')
            break;

        // Handle backspace
        if (ch == KEY_BACKSPACE || ch == 127)
        {
            if (!buffer.empty())
                buffer.pop_back();
        }
        // Handle printable characters
        else if (isprint(ch))
        {
            buffer.push_back((char)ch);
        }

        // Redraw input line with current text
        werase(winInput);
        box(winInput, 0, 0);
        mvwprintw(winInput, 1, 2, "> %s", buffer.c_str());
        wrefresh(winInput);
    }

    return buffer;
}

