# Script for loading log for interactive analysis

import matplotlib.pyplot as plt
import numpy as np
import pickle

def get_signal_series(src, fields):
    data_series = src
    for field in fields.split('.'):
        data_series = [getattr(msg, field) for msg in data_series]
    return np.array(data_series)

def append_group(data_struct, groups, elements, src, parents=[]):
    for group in groups:
        data_struct[group] = {}
        for element in elements:
            data_struct[group][elements] = get_signal_series(src, parents + [group, elements])


def import_log(log_filename):
    with open(log_filename, "rb") as fid:
        msgs = pickle.load(fid)
    
    data = {}
    
    #data['downlink_time'] = get_signal_series(msgs, 'downlink_timestamp_usec')
    
    data['imu_time'] = get_signal_series(msgs, 'sensor_measurement.imu_meas.timestamp_usec')
    data['accel_meas'] = {}
    data['gyro_meas'] = {}
    for elem in ['x','y','z']:
        data['accel_meas'][elem] = get_signal_series(msgs, 'sensor_measurement.imu_meas.accel_mpss.' + elem)
        data['gyro_meas'][elem] = get_signal_series(msgs, 'sensor_measurement.imu_meas.gyro_rads.' + elem)
    
    data['baro_meas'] = {}
    data['baro_meas']['time'] = get_signal_series(msgs, 'sensor_measurement.barometer_meas.timestamp_usec')
    data['baro_meas']['alt'] = get_signal_series(msgs, 'sensor_measurement.barometer_meas.altitude_m')
    
#    data['state_est'] = {}
#    data['state_est']['time'] = get_signal_series(msgs, 'estimated_state.timestamp_usec')
#    for vector in ['vel_b_mps','ang_vel_b_rads','euler_angle_rad','pos_E_m']:
#        data['state_est'][vector] = {}
#        for elem in ['x','y','z']:
#            data['state_est'][vector][elem] = get_signal_series(msgs, 'estimated_state.'+vector+'.'+elem)
    
    return data
    