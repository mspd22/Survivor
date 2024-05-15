//
//  coins.h
//  survivor
//
//  Created by Sree Pramod on 16/04/24.
//

#ifndef coins_h
#define coins_h

#include <vector>
#include <OpenGL/gl.h>
#include <GLFW/glfw3.h>

// Define the size of the coin
extern const float coinSize;

// Define the Coin struct
struct Coin {
    float x;
    float y;
    double spawnTime; // Time when the coin was spawned
    int type;
};

// Declare a vector of Coin objects
extern std::vector<Coin> coins;

// Declare the renderCoins function
void renderCoins(GLFWwindow* window, float playerX, float playerY, float playerSize, int& score);

#endif // coins_h
