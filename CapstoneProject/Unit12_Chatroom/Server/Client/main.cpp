// Shiv Patel
// CSC-284-WB
// Unit 12: Chatroom Client with Username + Commands

#include <iostream>
#include <thread>
#include <atomic>
#include <string>
#include <cstring>

#include "ChatClientUI.h"

using namespace std;

#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "Ws2_32.lib") // Link Winsock library
#else
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#define closesocket close
#endif


int main()
{
    // Default server connection info
    string ip = "127.0.0.1";// Default server IP
    int port = 54000;// Default server port

#ifdef _WIN32
    // Initialize Winsock (Windows only)
    WSADATA ws;
    WSAStartup(MAKEWORD(2, 2), &ws);
#endif

    // Create a TCP socket
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        cerr << "Failed to create socket.\n";
        
        return 1;
    }

    // Fill in server address information
    sockaddr_in server{};
    server.sin_family = AF_INET;
    server.sin_port = htons(port);
    inet_pton(AF_INET, ip.c_str(), &server.sin_addr);

    // Attempt to connect to the server
    if (connect(sock, (sockaddr*)&server, sizeof(server)) < 0) {
        cerr << "Failed to connect to server.\n";
        closesocket(sock);
        
        return 1;
    }

    // Create the curses-based UI
    ChatClientUI ui;
    ui.addMessage("Connected to server.");
    ui.addMessage("Enter username:");

    // Ask the user to enter a username in the input box
    string username = ui.getInput();
    if (username.empty()) {
        username = "anon"; // Server will assign anon#### if needed
    }

    // Send a command to the server to set the username
    string setCmd = ".SETNAME " + username;
    send(sock, setCmd.c_str(), (int)setCmd.size(), 0);

    // Flag used to control both threads
    atomic<bool> running(true);


    // Background thread: receives server messages
    thread receiver([&]() {
        char buffer[4096];

        while (running) {
            memset(buffer, 0, sizeof(buffer));
            int bytes = recv(sock, buffer, sizeof(buffer), 0);

            // If server disconnects or error occurs
            if (bytes <= 0) {
                ui.addMessage("Server disconnected.");
                running = false;
               
                break;
            }

            // Display incoming message in the UI
            ui.addMessage(string(buffer, bytes));
            ui.refreshAll();
        }
        });

  
    // Main thread: handles user input and sending
    while (running) {
        // Get input from the UI input box
        string input = ui.getInput();

        // Required .EXIT command
        if (input == ".EXIT" || input == ".exit") {
            send(sock, input.c_str(), (int)input.size(), 0);
            running = false;
            break;
        }

        // Optional quit shortcut
        if (input == "quit" || input == "QUIT") {
            running = false;
            break;
        }

        // Send any other input to the server
        // This includes:
        //   .CREATE_ROOM <room>
        //   .JOIN_ROOM <room>
        //   .LIST_ROOMS
        //   normal chat messages
        if (!input.empty()) {
            send(sock, input.c_str(), (int)input.size(), 0);
        }
    }

    // Close the socket connection
    closesocket(sock);

    // Wait for receiver thread to finish
    if (receiver.joinable()) {
        receiver.join();
    }

#ifdef _WIN32
    // Clean up Winsock resources
    WSACleanup();
#endif

    return 0;
}
