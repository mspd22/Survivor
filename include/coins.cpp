//
//  coins.cpp
//  survivor
//
//  Created by Sree Pramod on 16/04/24.
//

#include <iostream>
#include <vector>
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <GLFW/glfw3.h>
#include "coins.h"
#include "socket.h"

# define collectionMargin 15.0f
# define boundaryMargin 50.0f
// Define coin properties'
const float coinSize = 15.0f; // Size of the coin

// Initialize the vector of coins
std::vector<Coin> coins;

// Function to generate a random float between min and max
static float randomFloat(float min, float max) {
    return min + static_cast<float>(rand()) / static_cast<float>(RAND_MAX / (max - min));
}


// Function to render coins and handle player interaction
void renderCoins(GLFWwindow* window, float playerX, float playerY, float playerSize, int& score) {
    // Generate new coins every 0.5 seconds
    static double lastCoinTime = glfwGetTime();
    double currentTime = glfwGetTime();
    
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    
    if (currentTime - lastCoinTime >= 1.5) {
        // Avoid spawning coins within 6 units from the boundary
        float coinX = randomFloat((-width / 2.0f) + 100.0, (width / 2.0f) - 100.0);
        float coinY = randomFloat((-height / 2.0f) + 50.0, (height / 2.0f) - 50.0);
        int type = 2;
        
        if(randomFloat(0.0f, 1.0f) <= 0.9) type = 1;

        coins.push_back({coinX, coinY, currentTime, type}); // Add the new coin at the end of the vector
        lastCoinTime = currentTime;
    }

    // Render coins and handle player interaction
    
    auto it = coins.begin();
    while (it != coins.end()) {
        float coinX = it->x;
        float coinY = it->y;
        double coinSpawnTime = it->spawnTime; // Retrieve spawn time of the coin

        // Check for player and coin collision
        if (playerX + (playerSize / 2) + collectionMargin > coinX - coinSize / 2 &&
            playerX - (playerSize / 2) - collectionMargin < coinX + coinSize / 2 &&
            playerY + (playerSize / 2) + collectionMargin > coinY - coinSize / 2 &&
            playerY - (playerSize / 2) - collectionMargin < coinY + coinSize / 2){
            // Player collected the coin, increase score
            
            notifyCoinCollected();
            
            if(it->type == 1) score += 10;
            
            else score += 100;
            
            // Remove the collected coin
            it = coins.erase(it);

            // Add a new coin to the end of the vector
            float newCoinX = randomFloat((-width / 2.0f) + boundaryMargin, (width / 2.0f) - boundaryMargin);
            float newCoinY = randomFloat((-height / 2.0f) + boundaryMargin, (height / 2.0f) - boundaryMargin);
            coins.push_back({newCoinX, newCoinY, currentTime,1});

        } else {
            // Remove coins that have been on the screen for more than 10 seconds
            if((it->type == 1 && currentTime - coinSpawnTime >= 10.0) || (it->type == 2 && currentTime - coinSpawnTime >= 5.0)) {
                it = coins.erase(it);
            } else {
                // Draw the coin
                if(it->type == 1) glColor3f(1.0f, 1.0f, 0.0f); // Yellow color
                
                else glColor3f(0.4f, 0.7f, 1.0f);
                
                glBegin(GL_TRIANGLE_FAN);
                glVertex2f(coinX, coinY); // Center of the coin
                for (int i = 0; i <= 360; ++i) {
                    float angle = static_cast<float>(i) * 3.14159f / 180.0f; // Convert degrees to radians
                    float x = coinX + (coinSize / 2.0f) * cosf(angle);
                    float y = coinY + (coinSize / 2.0f) * sinf(angle);
                    glVertex2f(x, y);
                }
                glEnd();
                
                // Increment the iterator if the coin has not been removed
                ++it;
            }
        }
    }
}
