#include "ChatClientUI.h"

// Window class - simple wrapper for a curses window
Window::Window(int h, int w, int y, int x)
{
    height = h;
    width = w;
    starty = y;
    startx = x;

    win = newwin(height, width, starty, startx);
    box(win, 0, 0);
    wrefresh(win);
}

Window::~Window() {
    
    if (win != NULL) {
        delwin(win);
    }
}

WINDOW* Window::getWindow() {
    
    return win;
}

void Window::clearInside() {
   
    // Clear just the inside space so borders stay
    for (int y = 1; y < height - 1; y++) {
        
        for (int x = 1; x < width - 1; x++) {
            mvwaddch(win, y, x, ' ');
        }
    }
}

void Window::drawBorder() {
    box(win, 0, 0);
}

void Window::refreshWindow() {
    wrefresh(win);
}

// MessageArea - shows chat text
MessageArea::MessageArea(int h, int w, int y, int x)
    : Window(h, w, y, x)
{
    scrollOffset = 0;
}

void MessageArea::addMessage(const string& msg) {
    messages.push_back(msg);
    scrollOffset = 0; // Always scroll to bottom when new msg appears
}

void MessageArea::scrollUp() {
   
    int maxVisible = height - 2;
    int maxOffset = (int)messages.size() - maxVisible;

    if (maxOffset < 0) maxOffset = 0;

    if (scrollOffset < maxOffset) {
        scrollOffset++;
    }
}

void MessageArea::scrollDown() {
   
    if (scrollOffset > 0) {
        scrollOffset--;
    }
}

void MessageArea::draw() {
   
    clearInside();
    drawBorder();

    int maxVisible = height - 2;
    
    // Figure out which message to start drawing from
    int startIndex = (int)messages.size() - maxVisible - scrollOffset;
    
    if (startIndex < 0) startIndex = 0;

    int line = 1;

    for (int i = startIndex; i < (int)messages.size() && line <= maxVisible; i++, line++) {
        string text = messages[i];

        // Cut message if too long
        if ((int)text.size() > width - 2)
            text = text.substr(0, width - 3);

        mvwprintw(win, line, 1, "%s", text.c_str());
    }
}

// RoomList - list of chat rooms
RoomList::RoomList(int h, int w, int y, int x)
    : Window(h, w, y, x)
{
    selectedIndex = 0;
}

void RoomList::setRooms(const vector<string>& r) {
    rooms = r;

    if (selectedIndex >= (int)rooms.size()) {
        selectedIndex = 0;
    }
}

void RoomList::moveUp() {
    
    if (rooms.empty()) return;

    selectedIndex--;
    
    if (selectedIndex < 0)
        selectedIndex = (int)rooms.size() - 1;
}

void RoomList::moveDown() {
    
    if (rooms.empty()) return;

    selectedIndex++;
    
    if (selectedIndex >= (int)rooms.size())
        selectedIndex = 0;
}

string RoomList::getSelectedRoom() const {
    
    if (rooms.empty()) 
        return "";
    
    return rooms[selectedIndex];
}

void RoomList::draw() {
    
    clearInside();
    drawBorder();
    mvwprintw(win, 0, 2, " Rooms ");

    int line = 1;
    int maxVisible = height - 2;

    for (int i = 0; i < (int)rooms.size() && line <= maxVisible; i++, line++) {
        
        if (i == selectedIndex) {
            // Highlight selected room
            wattron(win, A_REVERSE);
            mvwprintw(win, line, 1, "%s", rooms[i].c_str());
            wattroff(win, A_REVERSE);
        }
        else {
            mvwprintw(win, line, 1, "%s", rooms[i].c_str());
        }
    }
}


// InputArea - lets user type messages
InputArea::InputArea(int h, int w, int y, int x)
    : Window(h, w, y, x)
{
}

void InputArea::clearBuffer() {
    buffer.clear();
}

const string& InputArea::getBuffer() const {
    
    return buffer;
}

void InputArea::handleCharacter(int ch) {
    
    // Handle backspace differently depending on terminal
    if (ch == KEY_BACKSPACE || ch == 127 || ch == 8) {
        
        if (!buffer.empty()) buffer.pop_back();
    }
    else if (isprint(ch)) {
        buffer.push_back((char)ch);
    }
}

void InputArea::draw() {
    
    clearInside();
    drawBorder();
    mvwprintw(win, 0, 2, " Message ");

    // Only show right part if too long
    string text = buffer;
    int maxLen = width - 3;
    
    if ((int)text.size() > maxLen)
        text = text.substr((int)text.size() - maxLen);

    mvwprintw(win, 1, 1, "%s", text.c_str());
}


// ChatClientUI - main controller for everything
ChatClientUI::ChatClientUI() {
    
    messageArea = NULL;
    roomList = NULL;
    inputArea = NULL;

    running = false;
    focusOnRooms = true; // Start by controlling the room list
}

ChatClientUI::~ChatClientUI() {
    
    delete messageArea;
    delete roomList;
    delete inputArea;
}

void ChatClientUI::initLayout() {
    
    int rows = LINES;
    int cols = COLS;

    int headerHeight = 3;
    int inputHeight = 3;
    int roomsWidth = 20;

    // Message area gets remaining height
    int messageHeight = rows - headerHeight - inputHeight;
    if (messageHeight < 5) messageHeight = 5;

    roomList = new RoomList(messageHeight, roomsWidth, headerHeight, 0);
    messageArea = new MessageArea(messageHeight, cols - roomsWidth, headerHeight, roomsWidth);
    inputArea = new InputArea(inputHeight, cols, rows - inputHeight, 0);

    // Add some sample rooms
    vector<string> rooms;
    rooms.push_back("General");
    rooms.push_back("Sports");
    rooms.push_back("Programming");
    rooms.push_back("Random");
    roomList->setRooms(rooms);

    // Add sample messages
    messageArea->addMessage("Welcome to the chat!");
    messageArea->addMessage("Use UP/DOWN to navigate rooms.");
    messageArea->addMessage("TAB switches focus.");
    messageArea->addMessage("Press ENTER to add your message.");
    messageArea->addMessage("Press 'q' to quit.");
}

void ChatClientUI::drawHeader() {
    
    // Clear header lines so redrawing is clean
    for (int y = 0; y < 3; y++) {
        move(y, 0);
        clrtoeol();
    }

    attron(A_BOLD);
    mvprintw(0, 2, "Advanced C++ Chat Client");
    attroff(A_BOLD);

    mvprintw(1, 2, "Room: %s", roomList->getSelectedRoom().c_str());
    mvprintw(1, COLS - 26, "[TAB] Switch | [q] Quit");

    // Show which section has focus so user knows what TAB did
    if (focusOnRooms)
        mvprintw(2, 2, "FOCUS: Rooms (UP/DOWN to change)");
    else
        mvprintw(2, 2, "FOCUS: Input (type + ENTER)");
}

void ChatClientUI::drawAll() {
    // We do NOT erase entire screen to avoid flashing
    drawHeader();

    messageArea->draw();
    messageArea->refreshWindow();

    roomList->draw();
    
    if (focusOnRooms) {
        wattron(roomList->getWindow(), A_BOLD);
        roomList->drawBorder();
        wattroff(roomList->getWindow(), A_BOLD);
    }
    roomList->refreshWindow();

    inputArea->draw();
   
    if (!focusOnRooms) {
        wattron(inputArea->getWindow(), A_BOLD);
        inputArea->drawBorder();
        wattroff(inputArea->getWindow(), A_BOLD);
    }
    inputArea->refreshWindow();

    refresh();
}

void ChatClientUI::run() {
    running = true;
    drawAll();

    int ch;
    
    while (running) {
        ch = getch();

        if (ch == 'q' || ch == 'Q') {
            running = false;
        }
        else {
            handleKey(ch);
        }

        drawAll();
    }
}

void ChatClientUI::handleKey(int ch) {
    
    if (ch == '\t') {
        // Switch focus with TAB
        focusOnRooms = !focusOnRooms;
        
        return;
    }

    if (focusOnRooms)
        handleRoomsKey(ch);
    else
        handleInputKey(ch);
}

void ChatClientUI::handleRoomsKey(int ch) {
    
    if (ch == KEY_UP)
        roomList->moveUp();
    else if (ch == KEY_DOWN)
        roomList->moveDown();
}

void ChatClientUI::handleInputKey(int ch) {
    
    if (ch == KEY_UP)
        messageArea->scrollUp();
    else if (ch == KEY_DOWN)
        messageArea->scrollDown();
    else if (ch == '\n') {
        // Add the typed message to the message list
        string msg = "[You in " + roomList->getSelectedRoom() + "]: " + inputArea->getBuffer();

        if (!inputArea->getBuffer().empty())
            messageArea->addMessage(msg);

        inputArea->clearBuffer();
    }
    else {
        inputArea->handleCharacter(ch);
    }
}
