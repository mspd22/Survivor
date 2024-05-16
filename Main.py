import tkinter as tk
import customtkinter as ctk
from PIL import Image, ImageTk
from run import start

# Create the main window
root = tk.Tk()
root.title("Survivor's Resolve")

# Maximize window
root.state('zoomed')

# Load the background image
background_image = Image.open("./assets/sprites/Untitled.png")

# Calculate aspect ratio
aspect_ratio = background_image.width / background_image.height

# Define the target width and height for resizing
target_width = root.winfo_screenwidth()  # Get the width of the screen
target_height = int(target_width / aspect_ratio)  # Maintain aspect ratio

# Resize the image
background_image = background_image.resize((target_width, target_height), Image.Resampling.LANCZOS)

# Convert the image to PhotoImage
background_photo = ImageTk.PhotoImage(background_image)
background_label = tk.Label(root, image=background_photo)
background_label.place(x=0, y=0, relwidth=1, relheight=1)

# Create a custom button
ctk.CTkButton(root, text='START GAME', text_color='#ffffff', width=150, height=10, font=('Comic Sans MS', 50, 'bold'),
              fg_color='#83d079', hover_color='#24bdc8', border_spacing=0,command=lambda: start(root)).place(x=300, y=500)

# Run the main event loop
root.mainloop()