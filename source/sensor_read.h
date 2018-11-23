/*
 * sensor_read.h - Library for updating sensor values from APM hardware
 * Created by Charlie Tytler  2018
 */
#ifndef SENSOR_READ_H
#define SENSOR_READ_H

//
// Initializes and calibrates barometer and IMU sensors.
//
// [return] Returns true if calibration was successful.
//
bool sensor_init_and_calibrate();

//
// Reads barometer sensor to update altitude based on barometric pressure.
//
// [input]  barometer:  Barometer sensor class to read measurement from.
// [output] altitude_m: Altitude of quadcopter in meters.
// [return] Returns true if sensor reading was successful and sensor is healthy.
//
bool read_baro_alt(AP_Baro_MS5611 barometer, float *altitude_m);

//
// Reads IMU sensor to update accelerometer and gyro measurements.
//
// [input]  imu:             Inertial Measurment Unit sensor class to read measurement from.
// [output] accel_meas_mpss: 3x1 Vector of acceleration measurements in m/s^2 from accelerometer.
// [output] gyro_meas_rads:  3x1 Vector of angular velocity measurements in rad/s from gyro.
// [return] Returns true if sensor reading was successful and sensor is healthy.
//
bool read_imu(const AP_InertialSensor imu, accel_t *accel_meas_mpss, gyro_t *gyro_meas_rads);

#endif
