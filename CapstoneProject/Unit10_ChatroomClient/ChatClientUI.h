#ifndef CHATCLIENTUI_H
#define CHATCLIENTUI_H

#include <iostream>
#include <string>
#include <vector>
#include "curses.h"   // Using PDCurses for terminal graphics

using namespace std;

/*
    This class is just a simple wrapper around a WINDOW*
    so we can draw borders and clear the inside easily.
*/
class Window {

protected:
    WINDOW* win;
    int height;
    int width;
    int starty;
    int startx;

public:
    Window(int h, int w, int y, int x);
    virtual ~Window();

    WINDOW* getWindow();
    void clearInside();
    void drawBorder();
    void refreshWindow();
};

/*
    This window shows chat messages.
    It also supports simple scrolling.
*/
class MessageArea : public Window {private:
    vector<string> messages;
    int scrollOffset;

public:
    MessageArea(int h, int w, int y, int x);

    void addMessage(const string& msg);
    void scrollUp();
    void scrollDown();
    void draw();
};

/*
    This window shows the list of chat rooms.
*/
class RoomList : public Window {

private:
    vector<string> rooms;
    int selectedIndex;

public:
    RoomList(int h, int w, int y, int x);

    void setRooms(const vector<string>& r);
    void moveUp();
    void moveDown();
    string getSelectedRoom() const;
    void draw();
};

/*
    This window is where the user types a message.
*/
class InputArea : public Window {

private:
    string buffer;

public:
    InputArea(int h, int w, int y, int x);

    void clearBuffer();
    const string& getBuffer() const;
    void handleCharacter(int ch);
    void draw();
};

/*
    This class controls the entire UI and handles keyboard input.
*/
class ChatClientUI {

private:
    MessageArea* messageArea;
    RoomList* roomList;
    InputArea* inputArea;

    bool running;
    bool focusOnRooms; // true = rooms list has focus

public:
    ChatClientUI();
    ~ChatClientUI();

    void initLayout();
    void drawHeader();
    void drawAll();
    void run();
    void handleKey(int ch);
    void handleRoomsKey(int ch);
    void handleInputKey(int ch);
};

#endif
