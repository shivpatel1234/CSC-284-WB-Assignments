#include <iostream>
#include <string>
#include <vector>
#include <thread>
#include <mutex>

#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "Ws2_32.lib") // link Winsock library
#else
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#define closesocket close
#endif

using namespace std;

// Global Client List
vector<SOCKET> clients; // store all connected client sockets
mutex clientsMutex; // protect client list access


// Broadcast Function
void broadcastMessage(const string& msg, SOCKET sender)
{
    lock_guard<mutex> lock(clientsMutex); // lock client list while sending

    for (SOCKET client : clients)
    {
        if (client == sender)
            
            continue; // skip sender so they don't get their own message

        send(client, msg.c_str(), msg.size(), 0); // send message to each client
    }
}

// Client Handler Thread
void handleClient(SOCKET clientSock)
{
    char buffer[4096];

    while (true)
    {
        memset(buffer, 0, sizeof(buffer)); // clear buffer
        int bytes = recv(clientSock, buffer, sizeof(buffer), 0); // receive message

        if (bytes <= 0)// client disconnected or error
        {
            cout << "Client disconnected.\n";

            // Remove client from list
            {
                lock_guard<mutex> lock(clientsMutex);
                clients.erase(remove(clients.begin(), clients.end(), clientSock), clients.end());
            }

            closesocket(clientSock);// close socket
            
            // end thread
            return;
        }

        string msg(buffer, bytes); // convert to string
        
        cout << "Received: " << msg << "\n";// print message to server console

        broadcastMessage(msg, clientSock);// send to all other clients
    }
}

int main()
{
#ifdef _WIN32
    WSADATA ws;
    WSAStartup(MAKEWORD(2, 2), &ws); // start Winsock
#endif

    SOCKET listening = socket(AF_INET, SOCK_STREAM, 0);// create listening socket

    sockaddr_in hint{};
    hint.sin_family = AF_INET;
    hint.sin_port = htons(54000); // port 54000
    inet_pton(AF_INET, "127.0.0.1", &hint.sin_addr);// bind to localhost

    bind(listening, (sockaddr*)&hint, sizeof(hint));// bind socket
    listen(listening, SOMAXCONN);// start listening for connections

    cout << "Server listening on 127.0.0.1:54000\n";

    while (true)
    {
        sockaddr_in client;
        int clientSize = sizeof(client);
        SOCKET clientSock = accept(listening, (sockaddr*)&client, &clientSize); // accept new client

        {
            lock_guard<mutex> lock(clientsMutex);
            clients.push_back(clientSock);// add client to list
        }

        cout << "Client connected.\n";

        thread clientThread(handleClient, clientSock); // create handler thread
        clientThread.detach();// detach so it runs independently
    }

#ifdef _WIN32
    WSACleanup(); // clean up Winsock
#endif

    return 0;
}
