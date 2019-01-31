import pickle
import ctypes
from SerialComms.c_struct_types import *


def state_estimate_execute(input_log):
    with open(input_log, "rb") as fid:
        logged_sensor_meas = pickle.load(fid)

    lib_state_estimation = ctypes.CDLL('../libraries/A0_Quadcopter_Libs/compiled_libs/state_estimation.so')
    lib_state_estimation.propagate_state_estimate.argtypes = [ctypes.POINTER(imu_meas_t), ctypes.POINTER(vehicle_state_t)]
    lib_state_estimation.measurement_update.argtypes = [ctypes.POINTER(sensor_measurement_t), ctypes.POINTER(vehicle_state_t)]


    estimated_state = vehicle_state_t()  #Initializes to zeros
    estimated_state.timestamp_usec = ctypes.c_float(logged_sensor_meas[0].sensor_measurement.imu_meas.timestamp_usec)
    logged_state = []
    logged_state.append(estimated_state)

    for msg in logged_sensor_meas:
        lib_state_estimation.propagate_state_estimate(msg.sensor_measurement.imu_meas, estimated_state)
        lib_state_estimation.measurement_update(msg.sensor_measurement, estimated_state)
        estimated_state.timestamp_usec = ctypes.c_float(msg.sensor_measurement.imu_meas.timestamp_usec)
        logged_state.append(estimated_state)

    return logged_state
