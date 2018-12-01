#!/usr/bin/env python3

# Playback a log with:
#   python playback_log.py -l logs/<logfilename>.dat

from PlotArtificialHorizon.gui_indicator import IndicatorWindow
import logging
import sys
import time
import pickle
import argparse


parser = argparse.ArgumentParser(description="Plays back a recorded log with options for displaying the data.")
parser.add_argument('-l', action="store", dest="log_filename", help="Specify path to log file")
parser.add_argument('--display', action="store_true", help="Display an attitude indicator figure")
parser.add_argument('--silent', action="store_true", help="Do not print downlink information to console")

args = parser.parse_args()

print(args.log_filename)

with open(args.log_filename, "rb") as fid:
    downlink_messages = pickle.load(fid)


if args.display:
    # Initialize Artificial Horizon indicator figure
    figure = IndicatorWindow()
    figure_update_counter = 0


system_time_offset = downlink_messages[0].downlink_timestamp_usec * 1E-6 - time.clock()

for msg in downlink_messages:

    while (time.clock() + system_time_offset) < (msg.downlink_timestamp_usec * 1E-6):
        pass

    received_estimated_state = msg.estimated_state

    if not args.silent:
        print("Attitude roll:%f pitch:%f yaw:%f time:%f"
                % (received_estimated_state.euler_angle_rad.x * 57.3,
                   received_estimated_state.euler_angle_rad.y * 57.3,
                   received_estimated_state.euler_angle_rad.z * 57.3,
                   received_estimated_state.timestamp_usec / 1.0E6))

    if args.display:
        # Update artificial horizon graphic
        figure_update_counter += 1
        if figure_update_counter >= 4:
            figure.update(received_estimated_state.euler_angle_rad.x * 57.3, received_estimated_state.euler_angle_rad.y * 57.3)
            figure_update_counter = 0


