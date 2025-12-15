#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <thread>
#include <mutex>
#include <algorithm>
#include <cstring>

#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "Ws2_32.lib")
#else
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#define closesocket close
typedef int SOCKET;
#endif

using namespace std;


// User class
// Represents a connected client
class User
{
public:
    SOCKET socket; // Client socket
    string username; // User's name
    string room;// Current room name

    User(SOCKET s)
    {
        socket = s;
        username = "";
        room = "";
    }
};



// Room class
// Represents a chat room
class Room
{
public:
    string name; // Room name
    vector<User*> users;// Users in this room

    Room(const string& n)
    {
        name = n;
    }

    // Add a user to the room
    void addUser(User* u)
    {
        users.push_back(u);
    }

    // Remove a user from the room
    void removeUser(User* u)
    {
        users.erase(remove(users.begin(), users.end(), u), users.end());
    }
};


// ChatServer class
// Main server logic
class ChatServer
{
private:
    SOCKET listeningSocket;// Server listening socket
    vector<User*> users; // All connected users
    map<string, Room*> rooms; // All chat rooms
    mutex mtx; // Protect shared data
    int anonCounter = 1; // Counter for anon usernames

public:
    ChatServer()
    {
        listeningSocket = INVALID_SOCKET;
    }

    // Start the server
    void start()
    {
        listeningSocket = socket(AF_INET, SOCK_STREAM, 0);

        sockaddr_in hint{};
        hint.sin_family = AF_INET;
        hint.sin_port = htons(54000);
        hint.sin_addr.s_addr = INADDR_ANY;

        bind(listeningSocket, (sockaddr*)&hint, sizeof(hint));
        listen(listeningSocket, SOMAXCONN);

        // Create default lobby room
        rooms["Lobby"] = new Room("Lobby");

        cout << "Server listening on port 54000\n";

        // Accept incoming clients
        while (true)
        {
            sockaddr_in client{};
            socklen_t clientSize = sizeof(client);

            SOCKET clientSocket = accept(listeningSocket, (sockaddr*)&client, &clientSize);

            User* user = new User(clientSocket);

            {
                lock_guard<mutex> lock(mtx);
                users.push_back(user);
            }

            // Handle client in its own thread
            thread t(&ChatServer::handleClient, this, user);
            t.detach();
        }
    }

    // Handle a single client
    void handleClient(User* user)
    {
        char buffer[4096];

        sendMessage(user, "[Server] Enter username:");

        while (true)
        {
            memset(buffer, 0, 4096);
            int bytes = recv(user->socket, buffer, 4096, 0);

            // Client disconnected
            if (bytes <= 0)
            {
                disconnectUser(user);
                return;
            }

            string msg(buffer);
            trim(msg);

            //Commands

            if (msg.rfind(".SETNAME", 0) == 0)
            {
                string name = msg.substr(8);
                trim(name);

                user->username = makeUniqueName(name);
                joinRoom(user, "Lobby");

                sendMessage(user, "[Server] Your username is " + user->username);
            }
            else if (msg.rfind(".CREATE_ROOM", 0) == 0)
            {
                string roomName = msg.substr(12);
                trim(roomName);

                lock_guard<mutex> lock(mtx);

                if (rooms.count(roomName) == 0)
                {
                    rooms[roomName] = new Room(roomName);
                    sendMessage(user, "[Server] Room created.");
                }
                else
                {
                    sendMessage(user, "[Server] Room already exists.");
                }
            }
            else if (msg.rfind(".JOIN_ROOM", 0) == 0)
            {
                string roomName = msg.substr(10);
                trim(roomName);

                lock_guard<mutex> lock(mtx);

                if (rooms.count(roomName))
                {
                    joinRoom(user, roomName);
                }
                else
                {
                    sendMessage(user, "[Server] Room does not exist.");
                }
            }
            else if (msg == ".LIST_ROOMS")
            {
                listRooms(user);
            }
            else if (msg == ".EXIT")
            {
                disconnectUser(user);
                return;
            }
            else
            {
                // Regular chat message
                sendToRoom(user, user->username + ": " + msg);
            }
        }
    }

    // Move user into a room
    void joinRoom(User* user, const string& roomName)
    {
        if (!user->room.empty())
        {
            rooms[user->room]->removeUser(user);
        }

        user->room = roomName;
        rooms[roomName]->addUser(user);

        sendToRoom(user, "[Server] " + user->username + " joined the room.");
    }

    // Send message to all users in the same room
    void sendToRoom(User* sender, const string& msg)
    {
        Room* room = rooms[sender->room];

        for (User* u : room->users)
        {
            if (u != sender)
            {
                sendMessage(u, msg);
            }
        }
    }

    // Send a message to one user
    void sendMessage(User* user, const string& msg)
    {
        send(user->socket, msg.c_str(), msg.size(), 0);
    }

    // Send list of rooms to a user
    void listRooms(User* user)
    {
        string out = "[Server] Rooms:\n";

        for (auto& pair : rooms)
        {
            out += " - " + pair.first + "\n";
        }

        sendMessage(user, out);
    }

    // Disconnect and clean up a user
    void disconnectUser(User* user)
    {
        if (!user->room.empty())
        {
            rooms[user->room]->removeUser(user);
        }

        closesocket(user->socket);

        lock_guard<mutex> lock(mtx);
        users.erase(remove(users.begin(), users.end(), user), users.end());

        delete user;
    }

    // Ensure usernames are unique
    string makeUniqueName(const string& requested)
    {
        if (requested.empty())
        {
            return "anon" + to_string(anonCounter++);
        }

        for (User* u : users)
        {
            if (u->username == requested)
            {
                return "anon" + to_string(anonCounter++);
            }
        }

        return requested;
    }

    // Trim whitespace from a string
    void trim(string& s)
    {
        s.erase(0, s.find_first_not_of(" \t\r\n"));
        s.erase(s.find_last_not_of(" \t\r\n") + 1);
    }
};



// main
int main()
{
#ifdef _WIN32
    // Initialize Winsock
    WSADATA data;
    WSAStartup(MAKEWORD(2, 2), &data);
#endif

    // Start the chat server
    ChatServer server;
    server.start();

#ifdef _WIN32
    // Clean up Winsock
    WSACleanup();
#endif

    return 0;
}
