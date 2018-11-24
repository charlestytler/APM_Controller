/*
 * imu_fast_loop.c - Library for "fast loop" functions which will be aligned with
 *     the IMU sampling rate.
 * Created by Charlie Tytler  2018
 */

#include "imu_fast_loop.h"

#include "sensor_read.h"
#include "global_data.h"

#include <AP_HAL.h>
#include <AP_HAL_AVR.h>
#include <AP_InertialSensor.h>
#include <AP_Math.h>


AP_InertialSensor imu;
static uint32_t   prev_frame_time_usec = 0U;

void imu_fast_loop_init()
{
    imu.init(AP_InertialSensor::COLD_START, AP_InertialSensor::RATE_100HZ);
}

uint32_t imu_fast_loop_exectue()
{
    // Hardware Abstraction Layer (HAL) for APM Board
    const AP_HAL::HAL &hal = AP_HAL_BOARD_DRIVER;

    // Wait for next IMU sample.
    imu.wait_for_sample();
    uint32_t frame_start_time_usec = hal.scheduler->micros();

    imu.update();
    Vector3f accel_mps = imu.get_accel();
    Vector3f gyro_rads = imu.get_gyro();

    imu_meas_t imu_meas;
    imu_meas.accel_mps.x    = accel_mps.x;
    imu_meas.accel_mps.y    = accel_mps.y;
    imu_meas.accel_mps.z    = accel_mps.z;
    imu_meas.gyro_rads.x    = gyro_rads.x;
    imu_meas.gyro_rads.y    = gyro_rads.y;
    imu_meas.gyro_rads.z    = gyro_rads.z;
    imu_meas.timestamp_usec = frame_start_time_usec;


    uint32_t delta_t_s   = (float) (frame_start_time_usec - prev_frame_time_usec) / 1.0e6F;
    prev_frame_time_usec = frame_start_time_usec;

    // TODO: propogate state estimation with IMU measurement

    write_sensor_imu_measurement(imu_meas);

    return frame_start_time_usec;
}
