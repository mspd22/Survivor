//
//  socket.h
//  survivor
//
//  Created by Sree Pramod on 23/04/24.
//

#ifndef socket_h
#define socket_h

#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

extern int sock; // Declare the global socket variable

// Function to initialize the socket connection
void setupSocket();

// Function to notify a coin collection event
void notifyCoinCollected();

// Function to notify game over
void notifyGameOver();

#endif /* socket_h */
