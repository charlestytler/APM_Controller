/*
 * imu_fast_loop.c - Library for "fast loop" functions which will be aligned with
 *     the IMU sampling rate.
 * Created by Charlie Tytler  2018
 */

#include "imu_fast_loop.h"

#include <sensor_read.h>
#include <global_data.h>
#include <AP_InertialSensor.h>
#include <AP_HAL.h>

AP_InertialSensor imu;
const AP_HAL::HAL& hal = AP_HAL_BOARD_DRIVER;
static uint32_t fast_loop_timer_us = 0U;

void imu_fast_loop_init()
{
    imu.init(AP_InertialSensor::COLD_START, AP_InertialSensor::RATE_100HZ);
}

uint32_t imu_fast_loop_exectue()
{
    // Wait for next IMU sample.
    imu.wait_for_sample();
    uint32_t frame_start_time_us = hal.scheduler->micros();

    delta_t_s = (float)(frame_start_time - fast_loopTimer) / 1.0e6F;
    fast_loop_timer_us = frame_start_time_us;

    Vector3f accel_meas;
    Vector3f gyro_meas;
    read_imu(imu, &accel_meas, &gyro_meas);

    //TODO: propogate state estimation with IMU measurement

    return frame_start_time_us;
}
