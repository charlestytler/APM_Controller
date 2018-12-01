# Playback a log with:
#   python log_playback.py -l logs/<logfilename>.dat


from c_struct_types import *
import logging
import sys
import time
import pickle
import argparse


parser = argparse.ArgumentParser()
parser.add_argument('-l', action="store", dest="log_filename")
args = parser.parse_args()

print(args.log_filename)

downlink_messages = downlink_message_t()
with open(args.log_filename, "rb") as fid:
    downlink_messages = pickle.load(fid)


system_time_offset = downlink_messages[0].downlink_timestamp_usec * 1E-6 - time.clock()

for msg in downlink_messages:

    while (time.clock() + system_time_offset) < (msg.downlink_timestamp_usec * 1E-6):
        pass

    received_estimated_state = msg.estimated_state

    print("Attitude roll:%f pitch:%f yaw:%f time:%f"
                        % (received_estimated_state.euler_angle_rad.x * 57.3,
                        received_estimated_state.euler_angle_rad.y * 57.3,
                        received_estimated_state.euler_angle_rad.z * 57.3,
                        received_estimated_state.timestamp_usec / 1.0E6))

