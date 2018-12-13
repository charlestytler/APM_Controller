/*
 * state_estimation.cpp - Library to estimate quadcopter state using sensor measurement data read from global_data.
 * Created by Charlie Tytler  2018
 */

#ifndef STATE_ESTIMATION_H
#define STATE_ESTIMATION_H

#include "global_data.h"


//
// Initializes estimated_state to initial condition of zero velocity/rotation at x,y position of (0,0).
//
void state_estimation_init();

//
// Updates the estimated_state using measured sensor data.
//
void state_estimation_execute();

//
// Reads in IMU sensor data and propagates the estimated state with the angular rates and linear accelerations.
//
// input: imu_meas - The latest IMU measurement which includes gyro rates and a timestamp.
// input/outupt: estimated_state - Updates the estimated_state with integrated rates.
void propagate_state_estimate(const imu_meas_t *imu_meas, vehicle_state_t *estimated_state);

//
// Reads in sensor measurements and updates IMU-propagated estimate of state.
//
// input: sensor_measurement - Sensor measurements which will be compared against propagated state estimate.
// input/outupt: estimated_state - Updates the propagated estimated_state with the innovation from the measurements.
void measurement_update(const sensor_measurement_t *sensor_measurement, vehicle_state_t *estimated_state);


#endif
