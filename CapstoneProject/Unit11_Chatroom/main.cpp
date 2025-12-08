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
    string ip = "127.0.0.1"; // Default server IP
    int port = 54000;// Default server port


#ifdef _WIN32
    WSADATA ws;// Start Winsock
    WSAStartup(MAKEWORD(2, 2), &ws);
#endif


    int sock = socket(AF_INET, SOCK_STREAM, 0);// Create TCP socket


    sockaddr_in server{};// Server address structure
    server.sin_family = AF_INET;
    server.sin_port = htons(port);
    inet_pton(AF_INET, ip.c_str(), &server.sin_addr);


    connect(sock, (sockaddr*)&server, sizeof(server)); // Connect to server


    ChatClientUI ui;// Create UI
    ui.addMessage("Connected to server."); // Initial message


    atomic<bool> running(true);// Shared flag for threads

    // Background thread: receives messages from the server
    thread receiver([&]() {
        char buffer[4096];

        while (running) {
            memset(buffer, 0, sizeof(buffer));
            int bytes = recv(sock, buffer, sizeof(buffer), 0);

            // If server disconnects or error
            if (bytes <= 0) {
                ui.addMessage("Server disconnected.");
                running = false;

                break;
            }

            // Add incoming message to UI
            ui.addMessage(string(buffer, bytes));
            ui.refreshAll();
        }
        });

    // Main thread: handles user input and sends messages
    while (running) {

        string input = ui.getInput();// Get text from ui input box

        if (input == "quit") {// Quit command
            running = false;

            break;
        }

        send(sock, input.c_str(), input.size(), 0);// Send message to server
    }


    closesocket(sock);// Close client socket
    receiver.join();// Wait for receiver thread to finish


#ifdef _WIN32
    WSACleanup(); // Clean up Winsock
#endif

    return 0;
}
