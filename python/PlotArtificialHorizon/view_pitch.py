import math
import cairocffi as cairo

from PlotArtificialHorizon.common import *

def _draw_sky_and_ground(cr, pitch_deg=0, roll_deg=0):
    offset = 200
    cr.set_source_rgb(*color_sky)
    cr.rectangle(-offset, -offset, max_x + offset, cy + offset)
    cr.fill()

    cr.set_source_rgb(*color_ground)
    cr.rectangle(-offset, cy, max_x + offset, max_y + offset)
    cr.fill()

def _pitch_to_scale(deg):
    return deg * (.7 * middle_radius) / 30

def _trace_long_line(cr, deg):
    dx = .3 * middle_radius
    y = cy - _pitch_to_scale(deg)
    cr.move_to(cx - dx, y)
    cr.line_to(cx + dx, y)

def _trace_short_line(cr, deg):
    dx = .15 * middle_radius
    y = cy - _pitch_to_scale(deg)
    cr.move_to(cx - dx, y)
    cr.line_to(cx + dx, y)


def draw_pitch_indicator(cr, pitch_deg=0, roll_deg=0):
    cr.translate(cx, cy)
    cr.rotate(math.radians(-roll_deg))
    cr.translate(-cx, -cy)

    cr.translate(0, _pitch_to_scale(pitch_deg))

    _draw_sky_and_ground(cr)

    cr.translate(0, _pitch_to_scale(-pitch_deg))

    cr.translate(cx, cy)
    cr.rotate(math.radians(roll_deg))
    cr.translate(-cx, -cy)

    # draw lines

    cr.arc(cx, cy, 0.9 * middle_radius, 0, 2*math.pi)
    # cr.stroke()
    cr.clip()

    cr.translate(cx, cy)
    cr.rotate(math.radians(-roll_deg))
    cr.translate(-cx, -cy)

    cr.translate(0, _pitch_to_scale(pitch_deg))

    cr.set_source_rgb(*color_white)

    cr.set_line_width(4)
    cr.move_to(0, cy)
    cr.line_to(max_x, cy)

    # short line
    for d in range(-25, 26, 10):
        _trace_short_line(cr, d)

    # long line
    for d in range(-30, 31, 10):
        if d == 0:
            continue
        _trace_long_line(cr, d)

    cr.stroke()
    # cr.select_font_face("Sans-Serif", cairo.FontSlant.NORMAL,
    #   cairo.FontWeight.BOLD)
    cr.set_font_size(16)
    for d in range(-30, 31, 10):
        if d == 0:
            continue
        s = str(d)
        dx = .3 * middle_radius
        y = cy - _pitch_to_scale(d)
        cr.move_to(cx - dx - 30, y + 6)
        cr.show_text(s)
        cr.move_to(cx + dx + 4, y + 6)
        cr.show_text(s)

    cr.reset_clip()

    cr.translate(0, _pitch_to_scale(-pitch_deg))

    cr.translate(cx, cy)
    cr.rotate(math.radians(roll_deg))
    cr.translate(-cx, -cy)
