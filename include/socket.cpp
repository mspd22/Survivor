//
//  socket.cpp
//  survivor
//
//  Created by Sree Pramod on 23/04/24.
//

#include "socket.h"
#include <iostream>

int sock = -1; // Define the global socket variable
const int port = 50550;

// Function to initialize the socket connection
void setupSocket() {
    // Create the socket

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1) {
        std::cerr << "Failed to create socket\n";
        return;
    }
    // Define the server address and port
    sockaddr_in server;
    server.sin_family = AF_INET;
    server.sin_port = htons(port); // Port number must match the one in the Python script
    server.sin_addr.s_addr = inet_addr("127.0.0.1"); // Localhost IP address
 
    // Connect to the Python script
    if (connect(sock, (sockaddr*)&server, sizeof(server)) == -1) {
        std::cerr << "Failed to connect to the Python server\n";
        close(sock);
        sock = -1;
        return;
    }
}

// Function to notify Python script of coin collection event
void notifyCoinCollected() {
    if (sock != -1) {
        // Send a message to the Python script
        std::string message = "coin_collected";
        send(sock, message.c_str(), message.length(), 0);
    }
}

void notifyGameOver() {
    if (sock != -1) {
        // Send a message to the Python script
        std::string message = "game_over";
        send(sock, message.c_str(), message.length(), 0);
    }
}
