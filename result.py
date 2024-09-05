import tkinter as tk
from PIL import Image, ImageTk
import json

def final_window(score : int) -> None:
    # Create the main window
    root = tk.Tk()
    root.title("Survivor's Resolve")

        # Maximize window
    # root.state('zoomed')
    root.attributes('-fullscreen', True)

        # Load the background image
    background_image = Image.open("./assets/sprites/GO_screen.png")

        # Calculate aspect ratio
    aspect_ratio = background_image.width / background_image.height

        # Define the target width and height for resizing
    target_width = root.winfo_screenwidth()  # Get the width of the screen
    target_height = int(target_width / aspect_ratio)  # Maintain aspect ratio

        # Resize the image
    background_image = background_image.resize((target_width, target_height), Image.Resampling.LANCZOS)

        # Convert the image to PhotoImage
    background_photo = ImageTk.PhotoImage(background_image)

        # Create a label with the background image
    background_label = tk.Label(root, image=background_photo)
    background_label.place(x=0, y=0, relwidth=1, relheight=1)

    with open("./assets/persist/data.json", "r") as file:
            data = json.load(file)
    high_score = data["high_score"]


    if score > high_score:
        data["high_score"] = score
        high_score=score
        with open('./assets/persist/data.json','w') as file:
            json.dump(data,file)

    file.close()


    # root.attributes("-transparentcolor", "white")

    tk.Label(root, text=f"{high_score}", font=('Comic Sans MS', 50, 'bold'), borderwidth=0, relief="flat").place(x=350,y=490)
    tk.Label(root, text=f"{score}", font=('Comic Sans MS', 50, 'bold'), borderwidth=0, relief="flat").place(x=1150,y=490)

    # Run the main event loop
    root.mainloop()



