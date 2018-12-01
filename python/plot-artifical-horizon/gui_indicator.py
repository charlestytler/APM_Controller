import numpy as np
import tkinter

import draw_indicator_frame
from common import max_x, max_y


class A:
    def __init__(self, master):
        self.label=tkinter.Label(master)
        self.label.grid(row=0, column=0)

        self.count = 0
        self.roll_vec = np.linspace(1, 20, 50)
        self.pitch_vec = np.linspace(-20, 40, 50)
        self.update_label()

    def update_label(self):
        if self.count < 50:
            fn = "indicator"
            draw_indicator_frame.run(
                fn,
                roll=self.roll_vec[self.count],
                pitch=self.pitch_vec[self.count])
            photo = tkinter.PhotoImage(file=fn+".png")
            self.label.configure(image=photo)
            self.label.image = photo
            self.label.after(1, self.update_label)
            self.count += 1

root = tkinter.Tk()
A(root)
root.mainloop()
