//
//  enemy.cpp
//  survivor
//
//  Created by Sree Pramod on 21/04/24.
//

#include <iostream>
#include <map>
#include <cmath>
#include "enemy.h"
#include "socket.h"


// Map to store direction and start time for each RANDOM_MOVEMENT_ENEMY
std::map<int, std::pair<int, double>> enemyDirectionMap;  // Key: enemy index, Value: pair(direction, start time)

// Define the base speed of the enemy
const float baseEnemySpeed = 3.75f;
const float randomEnemySpeed = 2.0f;
int width, height;
// Initialize the vector of enemies
std::vector<Enemy> enemies;

// Function to generate a random float between min and max
static float randomFloat(float min, float max) {
    return min + static_cast<float>(rand()) / static_cast<float>(RAND_MAX / (max - min));
}

int quadrant(GLFWwindow* window, float playerX, float playerY){

    if(playerX > 0.0f && playerY > 0.0f) return 1;
    
    if(playerX < 0.0f && playerY > 0.0f) return 2;
    
    if(playerX < 0.0f && playerY < 0.0f) return 3;
    
    return 4;
}

bool reversedDirection(float height,float widht,int &direction,float &x,float &y){
    bool reversedDirection = false;
    if (x - 100.0f <= -width / 2) {  // Left boundary collision
        if (direction == 2) {  // If moving left
            direction = 3;  // Change direction to right
            reversedDirection = true;
        }
    } else if (x + 100.0f >= width / 2) {  // Right boundary collision
        if (direction == 3) {  // If moving right
            direction = 2;  // Change direction to left
            reversedDirection = true;
        }
    } else if (y - 15.0f <= -height / 2) {  // Bottom boundary collision
        if (direction == 1) {  // If moving down
            direction = 0;  // Change direction to up
            reversedDirection = true;
        }
    } else if (y + 15.0f >= height / 2) {  // Top boundary collision
        if (direction == 0) {  // If moving up
            direction = 1;  // Change direction to down
            reversedDirection = true;
        }
    }
    
    return reversedDirection;
}

// Function to spawn an enemy
void spawnEnemy(GLFWwindow* window, float playerX, float playerY) {
    // Instead of generating random spawn positions, set spawn positions to the origin (0, 0)
    int quad = quadrant(window, playerX, playerY);
    float minX = 0.0f,minY = 0.0f,maxX = 0.0f,maxY = 0.0f;
    
    glfwGetFramebufferSize(window, &width, &height);
    switch (quad) {
        case 1: // Top-right quadrant (1st quadrant)
            // Opposite is bottom-left quadrant (3rd quadrant)
            minX = -width/2.0f;
            minY = -height/2.0f;
            break;
        case 2: // Top-left quadrant (2nd quadrant)
            // Opposite is bottom-right quadrant (4th quadrant)
            maxX = width/2.0f;
            minY = -height/2.0f;
            break;
        case 3: // Bottom-left quadrant (3rd quadrant)
            // Opposite is top-right quadrant (1st quadrant)
            maxX = width/2.0f;
            maxY = height/2.0f;
            break;
        case 4: // Bottom-right quadrant (4th quadrant)
            // Opposite is top-left quadrant (2nd quadrant)
            minX = -width/2.0f;
            maxY = height/2.0f;
            break;
    }

    float spawnX = randomFloat(minX, maxX);
    float spawnY = randomFloat(minY, maxY);

    // Determine the type of enemy to spawn based on 80:20 ratio
    EnemyType type;
    float randValue = randomFloat(0.0f, 1.0f);
    if (randValue <= 0.8f) {
        // 80% chance to spawn a chasing enemy
        type = PREDICTIVE_CHASING_ENEMY;
    } else {
        // 20% chance to spawn a random movement enemy
        type = RANDOM_MOVEMENT_ENEMY;
        int enemyIndex = static_cast<int>(enemies.size()) - 1;
        int direction = rand() % 4;  // Random direction: 0 = up, 1 = down, 2 = left, 3 = right
        double startTime = glfwGetTime();
        
        // Store direction and start time in the map
        enemyDirectionMap[enemyIndex] = std::make_pair(direction, startTime);
    }

    // Spawn the enemy with the chosen type at the origin
    enemies.push_back({spawnX, spawnY, (float)glfwGetTime(),type});
}

// Function to render enemies and handle player interaction
void renderEnemies(GLFWwindow* window, float playerX, float playerY, float playerSpeed, float playerSize) {
    
    static double lastEnemySpawnTime = glfwGetTime(); // Time of the last enemy spawn
    double currentTime = glfwGetTime(); // Current time

       // Determine spawn interval
    double spawnInterval = 1.5; // Spawn one enemy every 1.5 seconds

    // Check if it's time to spawn an enemy
    if (currentTime - lastEnemySpawnTime >= spawnInterval) {
        spawnEnemy(window, playerX, playerY);
        lastEnemySpawnTime = currentTime;
    }
    
    float enemySpeed = baseEnemySpeed + currentTime * 0.01f;

    glColor3f(1.0f, 1.0f, 1.0f);

    auto it = enemies.begin();
    while (it != enemies.end()) {
        
        int enemyIndex = std::distance(enemies.begin(), it);
        float dx = playerX - it->x;
        float dy = playerY - it->y;
        float distance = sqrt(dx * dx + dy * dy);

        if (it->type == RANDOM_MOVEMENT_ENEMY) {
            auto [direction, startTime] = enemyDirectionMap[enemyIndex];
                       
                // Determine the current time and the elapsed time since the start of the movement
                double currentTime = glfwGetTime();
                double elapsedTime = currentTime - startTime;
                
                // Update the enemy's direction in the map if it has reversed
                if (reversedDirection(height,width,direction,it->x,it->y)) {
                    enemyDirectionMap[enemyIndex] = std::make_pair(direction, currentTime);
                }
                // If the elapsed time is less than 3 seconds, continue moving in the specified direction
                if (elapsedTime <= 2.0) {
                    
                    switch (direction) {
                        case 0: // Move up
                            it->y += randomEnemySpeed;
                            break;
                        case 1: // Move down
                            it->y -= randomEnemySpeed;
                            break;
                        case 2: // Move left
                            it->x -= randomEnemySpeed;
                            break;
                        case 3: // Move right
                            it->x += randomEnemySpeed;
                            break;
                    }
                    
                } else {
                    // After 3 seconds, reassign a new random direction and update the start time
                    int newDirection = rand() % 4; // 0 = up, 1 = down, 2 = left, 3 = right
                    double newStartTime = glfwGetTime();
                    enemyDirectionMap[enemyIndex] = std::make_pair(newDirection, newStartTime);
                }
            
        } else if (it->type == PREDICTIVE_CHASING_ENEMY) {
            // Handle predictive chasing enemy logic as before
            float predictedX = playerX + playerSpeed * (dx / distance);
            float predictedY = playerY + playerSpeed * (dy / distance);
            dx = predictedX - it->x;
            dy = predictedY - it->y;
            distance = sqrt(dx * dx + dy * dy);
            it->x += (dx / distance) * enemySpeed;
            it->y += (dy / distance) * enemySpeed;
        }

        // Draw the enemy
        glBegin(GL_QUADS);
        glVertex2f(it->x - 15.0f, it->y + 15.0f); // Top-left
        glVertex2f(it->x + 15.0f, it->y + 15.0f); // Top-right
        glVertex2f(it->x + 15.0f, it->y - 15.0f); // Bottom-right
        glVertex2f(it->x - 15.0f, it->y - 15.0f); // Bottom-left
        glEnd();

        // Check for player and enemy collision
        float halfEnemySize = 7.5f; // Adjusted enemy hitbox size
        float halfPlayerSize = playerSize / 2.0f - 5.0f; // Adjusted player hitbox size
        if (playerX + halfPlayerSize > it->x - halfEnemySize &&
            playerX - halfPlayerSize < it->x + halfEnemySize &&
            playerY + halfPlayerSize > it->y - halfEnemySize &&
            playerY - halfPlayerSize < it->y + halfEnemySize) {
            // After detecting collision between player and enemy
            double endTime = glfwGetTime();
            
            notifyGameOver();
            while (glfwGetTime() - endTime <= 2.5f) {
                glfwPollEvents();


                // Calculate the shrinking window effect
                double elapsedTime = glfwGetTime() - endTime;
                float shrinkFactor = static_cast<float>(elapsedTime / 2.5f); // Normalize the elapsed time to [0, 1]
                
                // Draw black rectangles shrinking towards the center
                glColor3f(0.0f, 0.0f, 0.0f);
                glBegin(GL_QUADS);

                // Top border
                glVertex2f(-width / 2, height / 2);
                glVertex2f(width / 2, height / 2);
                glVertex2f(width / 2, height / 2 - (height / 2) * shrinkFactor);
                glVertex2f(-width / 2, height / 2 - (height / 2) * shrinkFactor);

                // Bottom border
                glVertex2f(-width / 2, -height / 2);
                glVertex2f(width / 2, -height / 2);
                glVertex2f(width / 2, -height / 2 + (height / 2) * shrinkFactor);
                glVertex2f(-width / 2, -height / 2 + (height / 2) * shrinkFactor);

                // Left border
                glVertex2f(-width / 2, height / 2);
                glVertex2f(-width / 2 + (width / 2) * shrinkFactor, height / 2);
                glVertex2f(-width / 2 + (width / 2) * shrinkFactor, -height / 2);
                glVertex2f(-width / 2, -height / 2);

                // Right border
                glVertex2f(width / 2, height / 2);
                glVertex2f(width / 2 - (width / 2) * shrinkFactor, height / 2);
                glVertex2f(width / 2 - (width / 2) * shrinkFactor, -height / 2);
                glVertex2f(width / 2, -height / 2);
                
                glEnd();

                // Check if the space bar is pressed to skip the wait time
                if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
                    break;
                }

                // Swap buffers and continue
                glfwSwapBuffers(window);
            }
            
            
            // Close the window
            glfwSetWindowShouldClose(window, true);
            return;
           }

           // Check if the enemy should disappear after 7.5 seconds
           if (currentTime - it->spawnTime >= 7.5) {
               it = enemies.erase(it);
           } else {
               ++it;
           }
       }
   }
