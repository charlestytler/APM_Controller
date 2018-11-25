import ctypes
import serial
from telemetry_rx_functions import *
from c_struct_types import *
import logging
import sys


#Logging settings
logging.basicConfig(
    format='[%(asctime)s.%(msecs)03d] %(message)s', datefmt='%H:%M:%S',
    level=logging.INFO,
    stream=sys.stdout)

# Get an example packet
port = serial.Serial('/dev/ttyS3', 57600)
if port.is_open:
    print('Port connected; receiving packets...')

# Read a set of packets
packet_counter = 0
port.reset_input_buffer()
get_next_packet(port)  # called to flush port of any old data
while packet_counter < 40:
    packet = get_next_packet(port)

    # Deserialize binary packet into ctype struct
    received_sensor_data = sensor_measurement_t()
    ctypes.memmove(ctypes.addressof(received_sensor_data),
                   packet, ctypes.sizeof(received_sensor_data))

    logging.info("IMU ax:%f ay:%f az:%f time:%f  Baro alt_m:%f time:%f"
                        % (received_sensor_data.imu_meas.accel_mps.x,
                        received_sensor_data.imu_meas.accel_mps.y,
                        received_sensor_data.imu_meas.accel_mps.z,
                        received_sensor_data.imu_meas.timestamp_usec / 1.0E6,
                        received_sensor_data.barometer_meas.altitude_m,
                        received_sensor_data.barometer_meas.timestamp_usec / 1.0E6))

    packet_counter += 1

port.close()
