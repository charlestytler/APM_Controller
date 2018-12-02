#!/usr/bin/env python3

import ctypes
import serial
from SerialComms.telemetry_rx_functions import *
from SerialComms.c_struct_types import *
from PlotArtificialHorizon.gui_indicator import IndicatorWindow
import matplotlib.pyplot as plt
import logging
import sys
import argparse


parser = argparse.ArgumentParser(description="Receives downlinked data from Quadcopter and displays and/or logs it.")
parser.add_argument('--log', action="store_true", help="Log downlinked data to a file in logs/")
parser.add_argument('--display', action="store_true", help="Display an attitude indicator figure")
parser.add_argument('--silent', action="store_true", help="Do not print downlink information to console")
parser.add_argument('--limit', action="store", dest="max_packet_num", default=2000, help="Specify the maximum number of packets to read; 0 for no limit")
parser.add_argument('--port', action="store", dest="serial_port", default='/dev/ttyS3', help="Specify the port to read from (e.g. /dev/ttyS3 on Linux or COM3 on Windows)")
parser.add_argument('--plotx', action="store", dest="plotx", default="downlink_timestamp_usec", help="Specify x-axis value to plot (e.g. sensor_measurement.timestamp_usec)")
parser.add_argument('--ploty', action="store", dest="ploty", help="Specify y-axis value to plot (e.g. sensor_measurement.timestamp_usec)")
parser.add_argument('--plotyy', action="store", dest="plotyy", help="Specify a 2nd y-axis value to plot (colored red)")
parser.add_argument('--yscale', action="store", dest="yscale", default=1.0, help="Specify a scaling factor for y-axis plotted values")
args = parser.parse_args()


# Limiting the total number of packets read
max_packet_num = float(args.max_packet_num)
print(max_packet_num)
if max_packet_num < 1:
    max_packet_num = float('inf')


#Logging settings
logging.basicConfig(
    format='[%(asctime)s.%(msecs)03d] %(message)s', datefmt='%H:%M:%S',
    level=logging.INFO,
    stream=sys.stdout)

# Establish connection with serial port
port = serial.Serial(args.serial_port, 57600)
if port.is_open:
    print('Port connected; receiving packets...')
else:
    raise Exception('Unable to connect to port on ' + args.serial_port)

# Attitude Indicator window
if args.display:
    figure = IndicatorWindow()
    figure_update_counter = 0

# Function for parsing fieldnames from argument strings
if args.ploty:
    def get_signal(src, fields):
        # For src="structure" and fields="field1.field2"
        # will return the object structure.field1.field2
        return_object = src
        for field in fields.split('.'):
            return_object = getattr(return_object, field)
        return return_object

    plotfig = plt.figure()
    plotaxes = plotfig.add_subplot(111)
    plt.ion()
    plt.show()
    plot_update_counter = 0


# Logging data structure
stored_messages = []

# Read a set of packets
packet_counter = 0
port.reset_input_buffer()
get_next_packet(port)  # called to flush port of any old data


while packet_counter < max_packet_num:

    ################ Packet Read and Deserialization ######################
    packet = get_next_packet(port)
    packet_counter += 1

    received_msg = downlink_message_t()
    ctypes.memmove(ctypes.addressof(received_msg), packet, ctypes.sizeof(received_msg))

    received_sensor_data = sensor_measurement_t()
    received_estimated_state = vehicle_state_t()
    received_sensor_data = received_msg.sensor_measurement
    received_estimated_state = received_msg.estimated_state


    ############  Printing Contents of Downlink Messages ##################

    if not args.silent:
    #logging.info("IMU ax:%f ay:%f az:%f time:%f  Baro alt_m:%f time:%f"
    #                    % (received_sensor_data.imu_meas.accel_mps.x,
    #                    received_sensor_data.imu_meas.accel_mps.y,
    #                    received_sensor_data.imu_meas.accel_mps.z,
    #                    received_sensor_data.imu_meas.timestamp_usec / 1.0E6,
    #                    received_sensor_data.barometer_meas.altitude_m,
    #                    received_sensor_data.barometer_meas.timestamp_usec / 1.0E6))

        logging.info("Attitude roll:%f pitch:%f yaw:%f time:%f"
                            % (received_estimated_state.euler_angle_rad.x * 57.3,
                            received_estimated_state.euler_angle_rad.y * 57.3,
                            received_estimated_state.euler_angle_rad.z * 57.3,
                            received_estimated_state.timestamp_usec / 1.0E6))

    ###############  Display of Attitude Indicator #########################
    # Update artificial horizon graphic
    if args.display:
        figure_update_counter += 1
        if figure_update_counter >= 4:
            figure.update(received_estimated_state.euler_angle_rad.x * 57.3, received_estimated_state.euler_angle_rad.y * 57.3)
            figure_update_counter = 0


    ############### Plotting of Downlinked Content ########################
    if args.ploty:
        plot_update_counter += 1
        plot_update_value = 5
        if args.plotyy:
            plot_update_value = 10
        if plot_update_counter >= 5:
            plotaxes.plot(received_msg.downlink_timestamp_usec * 1.0E6, float(args.yscale) * get_signal(received_msg, args.ploty), 'b.')
            if args.plotyy:
                plotaxes.plot(received_msg.downlink_timestamp_usec * 1.0E6, float(args.yscale) * get_signal(received_msg, args.plotyy), 'r.')
            plt.pause(0.0001)
            plot_update_counter = 0

    ###############  Logging of Downlink Messages #########################
    if args.log:
        stored_messages.append(received_msg)

port.close()


############# Storage of Logged Downlinks in Pickle File #################
if args.log:
    import os
    import pickle
    import datetime
    datetime_suffix = datetime.datetime.now().strftime("%Y_%m_%d_%H%M%S")
    with open(os.path.join("logs", "logged_downlinks_" + datetime_suffix + ".pkl"),"wb") as fid:
        pickle.dump(stored_messages, fid)


############ Allow interactive view of plot before closing ##############
if args.ploty:
    plt.show(block=True)
