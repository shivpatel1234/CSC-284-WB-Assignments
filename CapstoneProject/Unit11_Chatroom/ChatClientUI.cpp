#include "ChatClientUI.h"
#include <algorithm>

ChatClientUI::ChatClientUI() {

    initscr(); // initialize curses screen
    cbreak(); // immediate key input (no Enter needed)
    noecho(); // do not display typed characters automatically
    keypad(stdscr, TRUE);// enable arrow keys
    start_color();// enable color usage

    init_pair(1, COLOR_YELLOW, COLOR_BLACK); // color for messages
    init_pair(2, COLOR_CYAN, COLOR_BLACK); // color for rooms
    init_pair(3, COLOR_WHITE, COLOR_BLACK); // color for header
    init_pair(4, COLOR_GREEN, COLOR_BLACK); // color for input text
    init_pair(5, COLOR_MAGENTA, COLOR_BLACK); // color for selected room highlight

    getmaxyx(stdscr, maxY, maxX); // get screen size

    roomWidth = maxX / 4; // rooms take 1/4 of screen
    messageWidth = maxX - roomWidth - 3; // message window takes the rest

    winHeader = newwin(3, maxX, 0, 0); // top header window
    winMessages = newwin(maxY - 6, messageWidth, 3, 0); // message window
    winRooms = newwin(maxY - 6, roomWidth, 3, messageWidth + 2); // room list window
    winInput = newwin(3, maxX, maxY - 3, 0); // bottom input box

    rooms = { "General", "Sports", "Programming", "Music", "Random" }; // room names

    for (auto& room : rooms)
        roomMessages[room] = {}; // create a message list for each room

    drawHeader(); // draw all UI sections
    drawRooms();
    drawMessages();
    drawInput();
}

ChatClientUI::~ChatClientUI() {
    endwin(); // restore terminal state
}

void ChatClientUI::addMessage(const string& msg) {

    string room = rooms[selectedRoom]; // add message to current room
    roomMessages[room].push_back(msg);

    if (scrollOffset > 0) // adjust scroll if user has scrolled up
        scrollOffset--;

    drawMessages(); // redraw message window
}

void ChatClientUI::drawHeader() {

    wattron(winHeader, COLOR_PAIR(3)); // white color
    box(winHeader, 0, 0); // draw border
    mvwprintw(winHeader, 1, 2, "Welcome to the Chat!"); // header text
    wattroff(winHeader, COLOR_PAIR(3));
    wrefresh(winHeader); // display
}

void ChatClientUI::drawRooms() {

    werase(winRooms); // clear window
    box(winRooms, 0, 0); // draw border

    for (int i = 0; i < rooms.size(); i++) {

        // highlight selected room
        if (i == selectedRoom) { 
            wattron(winRooms, COLOR_PAIR(5) | A_REVERSE);
            mvwprintw(winRooms, i + 1, 2, "%s", rooms[i].c_str());
            wattroff(winRooms, COLOR_PAIR(5) | A_REVERSE);
        }
        else {
            wattron(winRooms, COLOR_PAIR(2)); // normal room color
            mvwprintw(winRooms, i + 1, 2, "%s", rooms[i].c_str());
            wattroff(winRooms, COLOR_PAIR(2));
        }
    }

    wrefresh(winRooms);// update screen
}

void ChatClientUI::drawMessages() {

    werase(winMessages); // clear message area
    box(winMessages, 0, 0); // border
    wattron(winMessages, COLOR_PAIR(1)); // message text color

    string room = rooms[selectedRoom]; // get current room
    vector<string>& msgs = roomMessages[room]; // access room message list

    int winHeight = maxY - 8; // total lines available for messages
    int total = msgs.size(); // number of messages

    int start = max(0, total - winHeight - scrollOffset); // scrolling logic
    int line = 1;

    for (int i = start; i < total - scrollOffset; i++) {
        mvwprintw(winMessages, line++, 2, "%s", msgs[i].c_str()); // print each message

        if (line >= winHeight) 
            
            break; // stop if window is full
    }

    wattroff(winMessages, COLOR_PAIR(1));
    wrefresh(winMessages);
}

void ChatClientUI::drawInput() {

    werase(winInput);
    box(winInput, 0, 0); // border
    mvwprintw(winInput, 1, 2, ">"); // input prompt
    wrefresh(winInput);
}

void ChatClientUI::handleKeypress(int ch) {

    // move room selection up
    if (ch == KEY_UP) {

        if (selectedRoom > 0) selectedRoom--;
        drawRooms();
        drawMessages();
    }
    // move room selection down
    else if (ch == KEY_DOWN) { 

        if (selectedRoom < rooms.size() - 1) selectedRoom++;
        drawRooms();
        drawMessages();
    }
    // PageUp scroll
    else if (ch == KEY_PPAGE) {
        scrollOffset++;
        drawMessages();
    }
    // PageDown scroll
    else if (ch == KEY_NPAGE) {

        if (scrollOffset > 0) scrollOffset--;
        drawMessages();
    }
}

string ChatClientUI::getInput() {

    drawInput(); // draw input box

    int ch;
    string buffer = "";

    while (true) {
        ch = wgetch(winInput); // read key from input window

        if (ch == 10) // Enter key

            break;

        else if (ch == KEY_UP || ch == KEY_DOWN || ch == KEY_PPAGE || ch == KEY_NPAGE)
            handleKeypress(ch); // arrow keys & page scrolling

        // delete last character
        else if (ch == KEY_BACKSPACE || ch == 127) {

            if (!buffer.empty()) {
                buffer.pop_back();
            }
        }
        // normal printable character
        else if (isprint(ch)) {
            buffer.push_back((char)ch);
        }

        // redraw input box
        werase(winInput);
        box(winInput, 0, 0);
        mvwprintw(winInput, 1, 2, "> %s", buffer.c_str());
        wrefresh(winInput);
    }

    return buffer; // return full input string
}

void ChatClientUI::refreshAll() {

    // redraw every window
    drawHeader();
    drawRooms();
    drawMessages();
    drawInput();
}
