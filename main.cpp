//
//  main.cpp
//  survivor
//
//  Created by Sree Pramod on 14/04/24.
//

#define GL_SILENCE_DEPRECATION

#include <iostream>
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <GLFW/glfw3.h>
#include "include/coins.h"
#include "include/enemy.h"
#include "include/socket.h"

// Define window dimensions
const int WINDOW_WIDTH = 1400;
const int WINDOW_HEIGHT = 1250;

// Define player character properties
const float playerSize = 30.0f; // Size of the square
float playerX = 0.0f; // Initial X position (center of the window)
float playerY = 0.0f; // Initial Y position (center of the window)
float speed = 5.0f; // Movement speed
int score = 0; // Player score

// Function to handle keyboard input for player movement
void handleInput(GLFWwindow* window) {
    // Handle window close
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }

    // Handle player movement
    // Move up
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        playerY += speed;
    }

    // Move left
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        playerX -= speed;
    }

    // Move down
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        playerY -= speed;
    }

    // Move right
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        playerX += speed;
    }

    // Bounds checking to prevent the square from moving outside of the window
    if (playerX - playerSize / 2 < -WINDOW_WIDTH / 2.0f) {
        playerX = -WINDOW_WIDTH / 2.0f + playerSize / 2;
    }
    if (playerX + playerSize / 2 > WINDOW_WIDTH / 2.0f) {
        playerX = WINDOW_WIDTH / 2.0f - playerSize / 2;
    }
    if (playerY - playerSize / 2 < -WINDOW_HEIGHT / 2.0f) {
        playerY = -WINDOW_HEIGHT / 2.0f + playerSize / 2;
    }
    if (playerY + playerSize / 2 > WINDOW_HEIGHT / 2.0f) {
        playerY = WINDOW_HEIGHT / 2.0f - playerSize / 2;
    }
}

// Function to render the player character with a texture
void renderPlayer() {
    // Set the color to red
    glColor3f(1.0f, 0.0f, 0.0f);
    
    // Draw the player character with the texture
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 1.0f); glVertex2f(playerX - playerSize / 2, playerY + playerSize / 2); // Top-left
    glTexCoord2f(1.0f, 1.0f); glVertex2f(playerX + playerSize / 2, playerY + playerSize / 2); // Top-right
    glTexCoord2f(1.0f, 0.0f); glVertex2f(playerX + playerSize / 2, playerY - playerSize / 2); // Bottom-right
    glTexCoord2f(0.0f, 0.0f); glVertex2f(playerX - playerSize / 2, playerY - playerSize / 2); // Bottom-left
    glEnd();
}

// Function to render the scene
void render(GLFWwindow* window) {
    
    static double startTime = glfwGetTime();
    // Clear the screen with green color
    glClearColor(0.11f, 0.27f, 0.06f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // Render player
    renderPlayer();

    // Render coins
    renderCoins(window, playerX, playerY, playerSize, score);
    
    // Render enemies
    
    if(glfwGetTime() - startTime > 3.0f){
        renderEnemies(window, playerX, playerY, speed, playerSize);
    }
}

int main() {
        
    // Initialize GLFW
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW\n";
        return -1;
    }

    // Obtain the primary monitor
    GLFWmonitor* primaryMonitor = glfwGetPrimaryMonitor();
    if (!primaryMonitor) {
        std::cerr << "Failed to get primary monitor\n";
        glfwTerminate();
        return -1;
    }

    // Obtain the video mode (resolution) of the primary monitor
    const GLFWvidmode* videoMode = glfwGetVideoMode(primaryMonitor);
    if (!videoMode) {
        std::cerr << "Failed to get video mode of primary monitor\n";
        glfwTerminate();
        return -1;
    }

    // Create a full-screen window on the primary monitor
    GLFWwindow* window = glfwCreateWindow(videoMode->width, videoMode->height, "Survivor", primaryMonitor, NULL);
    if (!window) {
        std::cerr << "Failed to create GLFW window\n";
        glfwTerminate();
        return -1;
    }
    
    setupSocket();
    
    
    // Make the window's context current
    glfwMakeContextCurrent(window);
    
    // Hide the mouse cursor
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

    glfwFocusWindow(window); // Focus on the current window

    // Main loop
    while (!glfwWindowShouldClose(window)) {
        
        // Poll for and process events
        glfwPollEvents();

        // Handle input
        handleInput(window);

        // Set up viewport
        int width, height;
        glfwGetFramebufferSize(window, &width, &height);
        glViewport(0, 0, width, height);

        // Render
        render(window);

        // Swap front and back buffers
        glfwSwapBuffers(window);
    }

    // Clean up GLFW
    glfwTerminate();
        
    // Send the close_connection message to the Python server before closing the socket
    if (sock != -1) {
        std::string close_message = "close_connection";
        send(sock, close_message.c_str(), close_message.length(), 0);
    }
    
    // Close the socket
    if (sock != -1) {
        close(sock);
    }
    
    return score;
}
