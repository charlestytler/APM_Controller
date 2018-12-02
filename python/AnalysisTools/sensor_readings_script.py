# Convenient mappings

from AnalysisTools.log_import import *
data = import_log('logs/imu_100hz_log.pkl')

# Sensor readings
t = data['imu_time'] * 1.0E-6
p = data['gyro_meas']['x']
q = data['gyro_meas']['y']
r = data['gyro_meas']['z']
ax = data['accel_meas']['x']
ay = data['accel_meas']['y']
az = data['accel_meas']['z']
baro_alt = data['baro_meas']['alt']
baro_t = data['baro_meas']['time']

# Estimated terms
#phi = data['euler_angle_rad']['x']
#the = data['euler_angle_rad']['y']
#psi = data['euler_angle_rad']['z']