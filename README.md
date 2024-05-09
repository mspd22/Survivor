## Survivor

This repository contains the source code for a 2D game called "Survivor". It's a game where the player attempts to survive by avoiding enemies while collecting coins. The game features network communication between a C++ game client and a Python server using sockets.

## Features:

- Basic 2D graphics with a background image and sprites
- Player movement and collision detection with enemies
- Coin collection with sound effects
- High score persistence using a JSON file
- Socket communication for sending game events (coin collection, game over) to a Python server that plays sound effects

## Dependencies:

- C++ compiler (e.g., GCC, Clang)
- Python 3
- Pillow (Python image processing library)
- customtkinter (optional, for enhanced Tkinter UI elements)

## Running the Game:

1. Make sure you have the required dependencies installed.
2. In the project directory, run the Python script for the GUI: `python main.py`
3. This will launch the game's graphical menu. Clicking the "START GAME" button will start the C++ game executable in the background and launch the Python socket server in a separate process.

**Note:**

- The C++ game executable is assumed to be named `survivor` and located in the same directory as the Python scripts. You might need to adjust the paths based on your actual setup.
