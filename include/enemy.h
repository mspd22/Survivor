//
//  enemy.h
//  survivor
//
//  Created by Sree Pramod on 21/04/24.
//

#ifndef enemy_h
#define enemy_h

#include <vector>
#include <OpenGL/gl.h>
#include <GLFW/glfw3.h>

// Define the speed of the enemy
extern const float enemySpeed;

// Define enemy types
enum EnemyType {
//    CHASING_ENEMY,
    PREDICTIVE_CHASING_ENEMY,
    RANDOM_MOVEMENT_ENEMY
};

// Define the Enemy struct
struct Enemy {
    float x, y; // Position of the enemy
    float spawnTime;
    EnemyType type; // Type of the enemy
};

// Declare a vector of Enemy objects
extern std::vector<Enemy> enemies;

// Declare functions to handle enemy behavior
void renderEnemies(GLFWwindow* window, float playerX, float playerY, float playerSpeed, float playerSize);
void spawnEnemy(GLFWwindow* window, EnemyType type);

#endif // enemy_h
