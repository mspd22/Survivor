import socket
from pygame import mixer

def start_socket_server():
    # Initialize Pygame's sound mixer
    mixer.init()

    # Create a socket server
    server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    server_socket.bind(('127.0.0.1', 50550))  # Bind to localhost on port 50550
    server_socket.listen(1)
    
    print("\nSocket server is listening on port 50550...")

    # Accept incoming connection
    try:
        conn, addr = server_socket.accept()
        print(f"\nConnected by {addr}")
    except Exception as e:
        print(f"\nError accepting connection: {e}")

    # Listen for incoming messages
    while True:
        data = conn.recv(1024).decode()
        
        # If no data, continue the loop
        if not data:
            continue

        # Check if the data is the triggering message to close the connection
        if data == "close_connection":
            print("\n\nReceived close_connection command. Closing connection.")
            conn.close()
            server_socket.close()
            break

        # Check if the message is a coin collection event
        if data == "coin_collected":
            # Play sound effect for coin collection
            mixer.music.load("./assets/sounds/coin1.mp3")  # Specify the path to the sound file
            mixer.music.play()

        if data == "game_over":
            # Play sound effect for coin collection
            mixer.music.load("./assets/sounds/gameover.mp3")  # Specify the path to the sound file
            mixer.music.play()

        # Handle other types of messages here (if any)
        # ...

    print("\n\nSocket server connection closed.")

if __name__ == "__main__":
    start_socket_server()
