import tkinter as tk

class CanvasAnimation:
    def __init__(self, root):
        self.root = root
        self.root.title("PA10: Canvas Animation")

        self.canvas = tk.Canvas(root, width=600, height=400, bg="#87CEEB")
        self.canvas.pack()

        self.car_dx = 3    # Car horizontal speed
        self.cloud_dx = 1  # Cloud horizontal speed

        self.draw_static_background()
        self.draw_moving_objects()
        
        self.canvas.bind("<Button-1>", self.teleport_car)

        self.animate()

    def draw_static_background(self):
        self.canvas.create_rectangle(0, 300, 600, 400, fill="#228B22", outline="")

        self.canvas.create_oval(500, 30, 570, 100, fill="#FFD700", outline="")

        self.canvas.create_rectangle(450, 240, 530, 300, fill="#CD853F", outline="black")
        self.canvas.create_polygon(440, 240, 490, 190, 540, 240, fill="#8B0000", outline="black")

        self.canvas.create_text(80, 40, text="Name: 邓昀凯\nID: 2025010912", 
                                font=("Arial", 12, "bold"), fill="black", justify=tk.LEFT)

    def draw_moving_objects(self):
        self.cloud_parts = [
            self.canvas.create_oval(50, 50, 120, 90, fill="white", outline=""),
            self.canvas.create_oval(80, 30, 150, 80, fill="white", outline=""),
            self.canvas.create_oval(110, 50, 180, 90, fill="white", outline="")
        ]

        self.car_parts = [
            self.canvas.create_rectangle(50, 250, 170, 290, fill="#FF4500", outline="black"),
            self.canvas.create_rectangle(75, 220, 145, 250, fill="#FF4500", outline="black"),
            self.canvas.create_rectangle(80, 225, 105, 250, fill="#87CEFA", outline="black"),
            self.canvas.create_rectangle(115, 225, 140, 250, fill="#87CEFA", outline="black"),
            self.canvas.create_oval(70, 280, 100, 310, fill="#333333", outline="black"),
            self.canvas.create_oval(120, 280, 150, 310, fill="#333333", outline="black")
        ]

    def teleport_car(self, event):
        coords = self.canvas.coords(self.car_parts[0])
        current_center_x = (coords[0] + coords[2]) / 2
        current_center_y = (coords[1] + coords[3]) / 2

        dx = event.x - current_center_x
        dy = event.y - current_center_y

        for part in self.car_parts:
            self.canvas.move(part, dx, dy)

    def animate(self):
        car_body_coords = self.canvas.coords(self.car_parts[0])

        if car_body_coords[2] >= 600 :
            self.car_dx = -3
        elif car_body_coords[0] <= 0 :
            self.car_dx = 3
            
        for part in self.car_parts:
            self.canvas.move(part, self.car_dx, 0)

        cloud_coords = self.canvas.coords(self.cloud_parts[-1])
        if cloud_coords[0] > 600:
            for part in self.cloud_parts:
                self.canvas.move(part, -800, 0)
        else:
            for part in self.cloud_parts:
                self.canvas.move(part, self.cloud_dx, 0)

        self.root.after(20, self.animate)

if __name__ == "__main__":
    root = tk.Tk()
    app = CanvasAnimation(root)
    root.mainloop()