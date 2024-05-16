import subprocess
import tkinter as tk
import customtkinter as ctk
from result import final_window

def run_xcode_executable(executable_path, args=None):
    # If no arguments are provided, use an empty list
    if args is None:
        args = []
    
    # Run the C++ executable with the specified arguments
    result = subprocess.run([executable_path] + args, capture_output=True, text=True)
    
    # Print the return code
    final_window(result.returncode)

def start_socket_server():
    # Start `socket_server.py` as a separate process
    socket_server_process = subprocess.Popen(['python', 'socket_server.py'])
    
    # Return the process object for later control if needed
    return socket_server_process

def start(root):
    root.destroy()
    # Start the socket server
    socket_server_process = start_socket_server()
        
    # Specify the path to the C++ executable
    executable_path = './survivor'
        
    # Specify command-line arguments to pass to the executable (if any)
    args = []  # Replace 'arg1' and 'arg2' with your arguments, or set to None if no arguments are required
        
    # Run the C++ executable and handle the output
    run_xcode_executable(executable_path, args)
        
    # Wait for the socket server process to finish
    # socket_server_process.wait()  # Uncomment if you want to wait for the server to finish
