import math

import numpy as np

max_x = 600
max_y = 600

cx = max_x/2
cy = max_y/2

middle_radius = (max_x - 150) / 2

color_sky = (np.array([36, 161, 254]) / 255.).tolist()
color_ground = (np.array([137, 100, 58]) / 255.).tolist()
color_black = (0, 0, 0)
color_yellow = (np.array([244, 240, 45]) / 255.).tolist()
color_white = (1, 1, 1)
color_orange = (np.array([253, 106, 2]) / 255.).tolist()

def cw_deg_to_xy(deg, dist=0):
    """
    Based on middle_radius
    0 deg = 12 hr hand, positive clockwise
    """
    # import ipdb; ipdb.set_trace()

    # minus flips the sign for rotation direction
    theta = math.pi/2 - math.radians(deg)
    r = middle_radius + dist

    dx = r * math.cos(theta)
    dy = r * math.sin(theta)

    x = cx + dx
    y = cy - dy

    return x, y
