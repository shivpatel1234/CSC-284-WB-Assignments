#pragma once
#include <string>
#include <vector>
#include <map>
#include <curses.h>

using namespace std;

class ChatClientUI {
private:

    WINDOW* winHeader;// top header window
    WINDOW* winMessages; // main message display window
    WINDOW* winRooms; // room list window
    WINDOW* winInput;// bottom input box

    int maxY, maxX; // terminal dimensions
    int roomWidth; // width of room list column
    int messageWidth; // width of message column

    int selectedRoom = 0; // index of currently highlighted room
    int scrollOffset = 0; // how far user has scrolled up in message list

    vector<string> rooms; // list of room names
    map<string, vector<string>> roomMessages;// messages stored separately for each room

public:

    ChatClientUI(); // constructor initializes UI windows
    ~ChatClientUI(); // destructor restores terminal state

    void addMessage(const string& msg); // add a new message to current room

    void refreshAll(); // redraw entire UI
    void drawHeader();// draw header window
    void drawRooms(); // draw list of rooms
    void drawMessages(); // draw messages for selected room
    void drawInput(); // draw input box

    string getInput(); // capture user text input
    void handleKeypress(int ch); // handle arrow keys, scrolling, etc.
};
