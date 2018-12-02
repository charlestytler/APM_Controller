/*
 * imu_fast_loop.h - Library for "fast loop" functions which will be aligned with
 *     the IMU sampling rate.
 * Created by Charlie Tytler  2018
 */
#ifndef IMU_FAST_LOOP_H
#define IMU_FAST_LOOP_H

#include <stdint.h>

//
// IMU intialization and setting of IMU sampling rate - and thus imu fast loop rate.
//
void imu_fast_loop_init();

//
// Scheduled task that runs every frame and aligns with the IMU sampling rate to force periodicity.
//
// [return] Returns the start time of the frame in microseconds.
uint32_t imu_fast_loop_exectue();


//
// Displays accelerometer and gyro offset and scaling values.
//
void display_offsets_and_scaling();

#endif
