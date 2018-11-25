import math

from common import *

_settings = {
    'thin': {
        'length': 18,
        'width_deg': 2,
        'thickness': 4
    },
    'thick': {
        'length': 25,
        'width_deg': 2.5,
        'thickness': 7
    }
}

def _trace_lines_symm(cr, deg, line_type):
    current_settings = _settings[line_type]
    l = current_settings['length']

    cr.move_to(*cw_deg_to_xy(deg))
    cr.line_to(*cw_deg_to_xy(deg, l))

    cr.move_to(*cw_deg_to_xy(-deg))
    cr.line_to(*cw_deg_to_xy(-deg, l))


def _make_triangle(cr, deg, line_type):
    current_settings = _settings[line_type]
    l = current_settings['length']
    w_deg = current_settings['width_deg']

    cr.move_to(*cw_deg_to_xy(deg))
    cr.line_to(*cw_deg_to_xy(deg - w_deg, l))
    cr.line_to(*cw_deg_to_xy(deg + w_deg, l))
    cr.fill()


def draw_bank_indicator(cr, roll=0):
    cr.translate(cx, cy)
    cr.rotate(math.radians(-roll))
    cr.translate(-cx, -cy)

    cr.set_source_rgb(*color_white)

    # 0
    _make_triangle(cr, 0, 'thick')

    # 45
    _make_triangle(cr, 45, 'thin')
    _make_triangle(cr, -45, 'thin')

    # 30, 60
    cr.set_line_width(_settings['thick']['thickness'])
    _trace_lines_symm(cr, 30, 'thick')
    _trace_lines_symm(cr, 60, 'thick')
    cr.stroke()

    # 10 and 20
    cr.set_line_width(_settings['thin']['thickness'])
    _trace_lines_symm(cr, 10, 'thin')
    _trace_lines_symm(cr, 20, 'thin')
    cr.stroke()

    # draw arc
    eps = 1
    cr.arc(
        cx,
        cy,
        middle_radius,
        -math.radians(150 + eps),
        -math.radians(30 - eps)
    )
    cr.stroke()

    # # DEBUG:
    cr.arc(cx, cy, 8, 0, 2*math.pi)
    cr.fill()

    cr.translate(cx, cy)
    cr.rotate(math.radians(roll))
    cr.translate(-cx, -cy)
