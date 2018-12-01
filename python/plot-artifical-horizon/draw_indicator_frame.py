import cairocffi as cairo
import math

from common import *

import view_pitch
import view_roll
import view_static

def main(cr, roll=0, pitch=0):
    cr.arc(cx, cy, max_x/2, 0, 2*math.pi)
    cr.clip()

    view_pitch.draw_pitch_indicator(cr, pitch_deg=pitch, roll_deg=roll)

    view_roll.draw_bank_indicator(cr, roll)

    view_static.draw_aircraft_indicator(cr)
    cr.save()


def run(fn, roll=-3, pitch=40):
    surface = cairo.SVGSurface(fn + ".svg", max_x, max_y)

    if roll > 60:
        roll = 60
    elif roll < -60:
        roll = -60

    if pitch > 30:
        pitch = 30
    elif pitch < -30:
        pitch = -30

    cr = cairo.Context(surface)
    with cr:
        main(cr, roll, pitch)
        surface.write_to_png(fn + ".png")

if __name__ == "__main__":
    fn = "indicator"
    run(fn)
    print('done')
