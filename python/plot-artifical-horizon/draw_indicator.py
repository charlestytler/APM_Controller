import math

from common import *

import view_pitch
import view_roll
import view_static

if __name__ == "__main__":
    import cairo
    fn = "indicator"
    with cairo.SVGSurface(fn + ".svg", max_x, max_y) as surface:
        cr = cairo.Context(surface)

        cr.arc(cx, cy, max_x/2, 0, 2*math.pi)
        cr.clip()

        roll = -3
        pitch = 40

        if roll > 60:
            roll = 60
        elif roll < -60:
            roll = -60

        if pitch > 30:
            pitch = 30
        elif pitch < -30:
            pitch = -30

        view_pitch.draw_pitch_indicator(cr, pitch_deg=pitch, roll_deg=roll)

        view_roll.draw_bank_indicator(cr, roll)

        view_static.draw_aircraft_indicator(cr)

        surface.write_to_png(fn + ".png")
    print('done')
