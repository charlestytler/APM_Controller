import numpy as np
import tkinter

import draw_indicator_frame
from common import max_x, max_y


class IndicatorWindow:
    def __init__(self):
        self.root = tkinter.Tk()
        self.label=tkinter.Label(self.root)
        self.label.grid(row=0, column=0)

    def update(self, roll, pitch):
        fn = "indicator"
        draw_indicator_frame.run(
            fn,
            roll=roll,
            pitch=pitch)
        photo = tkinter.PhotoImage(file=fn+".png")
        self.label.configure(image=photo)
        self.label.image = photo
        self.root.update()

    def exit():
        self.root.quit()

def example_main():
    """
    Must call ctrl+c from command line to exit window
    """
    window = IndicatorWindow() # keep this

    # temporary
    # ========
    loop_active = True
    max_count = 50
    roll_vec = np.linspace(1, 20, max_count)
    pitch_vec = np.linspace(-20, 40, max_count)
    counter = 0

    while loop_active:
        if counter < max_count:
            window.update(roll_vec[counter], pitch_vec[counter]) # keep this
        counter += 1

if __name__ == "__main__":
    example_main()
