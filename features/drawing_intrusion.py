import tkinter as tk
from tkinter import filedialog
import cv2
from PIL import Image, ImageTk
import json

class VideoBoxDrawer:
    def __init__(self, root):
        self.root = root
        self.root.title("Draw Boxes on First Frame")

        self.load_button = tk.Button(root, text="Load Video", command=self.load_video)
        self.load_button.pack(pady=5)

        self.save_button = tk.Button(root, text="Save Boxes", command=self.save_boxes, state=tk.DISABLED)
        self.save_button.pack(pady=5)

        self.canvas = tk.Canvas(root, bg="gray")
        self.canvas.pack()

        # Data
        self.image = None
        self.tk_image = None
        self.boxes = []
        self.start_x = None
        self.start_y = None
        self.rect = None
        self.canvas_image_id = None

    def load_video(self):
        video_path = filedialog.askopenfilename(filetypes=[("Video Files", "*.mp4 *.avi *.mov *.mkv")])
        if not video_path:
            return

        cap = cv2.VideoCapture(video_path)
        ret, frame = cap.read()
        cap.release()

        if not ret:
            print("Failed to read video")
            return

        # Save image dimensions
        self.image_height, self.image_width = frame.shape[:2]

        # Convert OpenCV BGR -> RGB
        frame_rgb = cv2.cvtColor(frame, cv2.COLOR_BGR2RGB)
        self.image = Image.fromarray(frame_rgb)
        self.tk_image = ImageTk.PhotoImage(self.image)

        # Resize canvas
        self.canvas.config(width=self.image.width, height=self.image.height)

        # Clear canvas
        self.canvas.delete("all")
        self.boxes.clear()
        self.save_button.config(state=tk.NORMAL)

        # Show image on canvas
        self.canvas_image_id = self.canvas.create_image(0, 0, anchor="nw", image=self.tk_image)

        # Bind mouse
        self.canvas.bind("<ButtonPress-1>", self.on_press)
        self.canvas.bind("<B1-Motion>", self.on_drag)
        self.canvas.bind("<ButtonRelease-1>", self.on_release)

    def on_press(self, event):
        self.start_x = event.x
        self.start_y = event.y
        self.rect = self.canvas.create_rectangle(self.start_x, self.start_y, event.x, event.y, outline="red", width=2)

    def on_drag(self, event):
        if self.rect:
            self.canvas.coords(self.rect, self.start_x, self.start_y, event.x, event.y)

    def on_release(self, event):
        x1, y1 = self.start_x, self.start_y
        x2, y2 = event.x, event.y
        box = {
            "x1": int(min(x1, x2)),
            "y1": int(min(y1, y2)),
            "x2": int(max(x1, x2)),
            "y2": int(max(y1, y2))
        }
        self.boxes.append(box)

    def save_boxes(self):
        with open("boxes.json", "w") as f:
            json.dump(self.boxes, f, indent=4)
        print("Saved boxes to boxes.json")

if __name__ == "__main__":
    root = tk.Tk()
    app = VideoBoxDrawer(root)
    root.mainloop()
