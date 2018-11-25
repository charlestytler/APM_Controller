from common import *

def draw_aircraft_indicator(cr):
    cr.set_source_rgb(*color_orange)
    cr.set_line_width(8)

    # center dot

    cr.arc(cx, cy, 8, 0, 2*math.pi)
    cr.fill()

    # lines for wings

    cr.move_to(cx - 50, cy + 10)
    cr.line_to(cx - 50, cy)
    cr.line_to(cx - 150, cy)

    cr.move_to(cx + 50, cy + 10)
    cr.line_to(cx + 50, cy)
    cr.line_to(cx + 150, cy)
    cr.stroke()

    cr.set_line_width(2)
    cr.move_to(*cw_deg_to_xy(0))
    cr.line_to(*cw_deg_to_xy(-3, -25))
    cr.line_to(*cw_deg_to_xy(3, -25))
    cr.line_to(*cw_deg_to_xy(0))
    cr.stroke()
