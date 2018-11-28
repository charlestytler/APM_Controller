import ctypes
import serial
from telemetry_rx_functions import *
from c_struct_types import *
import logging
import sys

def deserialize_buffer(src, dest):
    size_already_deserialized = 0 * ctypes.sizeof(dest[0])
    for dest_structure in dest:
        ctypes.memmove(ctypes.addressof(dest_structure), ctypes.addressof(src) + size_already_deserialized, ctypes.sizeof(dest_structure))
        size_already_deserialized += ctypes.sizeof(dest_structure)


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
while True: #packet_counter < 200:
    packet = get_next_packet(port)

    # Deserialize binary packet into ctype struct
    received_msg = downlink_message_t()
    ctypes.memmove(ctypes.addressof(received_msg), packet, ctypes.sizeof(received_msg))

    received_sensor_data = sensor_measurement_t()
    received_estimated_state = vehicle_state_t()
    received_sensor_data = received_msg.sensor_measurement
    received_estimated_state = received_msg.estimated_state


#    logging.info("IMU ax:%f ay:%f az:%f time:%f  Baro alt_m:%f time:%f"
#                        % (received_sensor_data.imu_meas.accel_mps.x,
#                        received_sensor_data.imu_meas.accel_mps.y,
#                        received_sensor_data.imu_meas.accel_mps.z,
#                        received_sensor_data.imu_meas.timestamp_usec / 1.0E6,
#                        received_sensor_data.barometer_meas.altitude_m,
#                        received_sensor_data.barometer_meas.timestamp_usec / 1.0E6))

    logging.info("Attitude roll:%f pitch:%f yaw:%f time:%f"
                        % (received_estimated_state.euler_angle_rad.x * 57.3,
                        received_estimated_state.euler_angle_rad.y * 57.3,
                        received_estimated_state.euler_angle_rad.z * 57.3,
                        received_estimated_state.timestamp_usec / 1.0E6))
    packet_counter += 1

port.close()
