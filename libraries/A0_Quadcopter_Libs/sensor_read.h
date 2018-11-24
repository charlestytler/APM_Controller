/*
 * sensor_read.h - Library for updating sensor values from APM hardware
 * Created by Charlie Tytler  2018
 */
#ifndef SENSOR_READ_H
#define SENSOR_READ_H

#include <AP_InertialSensor.h>
#include <stdbool.h>

//
// Initializes and calibrates barometer and IMU sensors.
//
// [return] Returns true if calibration was successful.
//
bool sensor_init_and_calibrate();

//
// Reads barometer sensor to update altitude based on barometric pressure and writes to global_data.
//
void read_baro_alt();

#endif
