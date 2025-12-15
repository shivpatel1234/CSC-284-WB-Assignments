#pragma once
#pragma once

#include <string>
#include <vector>
#include <map>

#include <curses.h>

using namespace std;



// ChatClientUI
// Handles all curses-based UI rendering
class ChatClientUI
{
private:

    // UI windows
    WINDOW* winHeader; // Top title bar
    WINDOW* winMessages;// Main chat message area
    WINDOW* winRooms;// Rooms panel (can remain empty)
    WINDOW* winInput; // Input box at the bottom

    // Terminal size
    int maxY;
    int maxX;

    // Layout sizes
    int roomWidth;
    int messageWidth;

    // Room navigation (not fully used yet)
    int selectedRoom = 0;
    int scrollOffset = 0;

    // Room names and stored messages
    vector<string> rooms;
    map<string, vector<string>> roomMessages;

public:

    // Constructor / Destructor
    ChatClientUI();
    ~ChatClientUI();

    // Add a message to the chat display
    void addMessage(const string& msg);

    // Redraw all UI windows
    void refreshAll();

    // Individual draw functions
    void drawHeader();
    void drawRooms();
    void drawMessages();
    void drawInput();

    // Get user input from the input window
    string getInput();

    // Optional key handling (not required)
    void handleKeypress(int ch);
};
