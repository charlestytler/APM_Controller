/*
 * imu_fast_loop.cpp - Library for "fast loop" functions which will be aligned with
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
    // Hardware Abstraction Layer (HAL) for APM Board
    const AP_HAL::HAL &hal = AP_HAL_BOARD_DRIVER;

    imu.init(AP_InertialSensor::COLD_START, AP_InertialSensor::RATE_100HZ);

    // run accel level
    imu.init_accel();

    // float roll_trim, pitch_trim;
    // AP_InertialSensor_UserInteractStream interact(hal.console);
    // imu.calibrate_accel(&interact, roll_trim, pitch_trim);

    // display results
    display_offsets_and_scaling();
}

uint32_t imu_fast_loop_exectue()
{
    // Hardware Abstraction Layer (HAL) for APM Board
    const AP_HAL::HAL &hal = AP_HAL_BOARD_DRIVER;

    // Wait for next IMU sample.
    imu.wait_for_sample();
    uint32_t frame_start_time_usec = hal.scheduler->micros();

    imu.update();
    Vector3f accel_mpss = imu.get_accel();
    Vector3f gyro_rads  = imu.get_gyro();

    imu_meas_t imu_meas;
    imu_meas.accel_mpss.x   = accel_mpss.x;
    imu_meas.accel_mpss.y   = accel_mpss.y;
    imu_meas.accel_mpss.z   = accel_mpss.z;
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


void display_offsets_and_scaling()
{
    // Hardware Abstraction Layer (HAL) for APM Board
    const AP_HAL::HAL &hal = AP_HAL_BOARD_DRIVER;

    Vector3f accel_offsets = imu.get_accel_offsets();
    Vector3f accel_scale   = imu.get_accel_scale();
    Vector3f gyro_offsets  = imu.get_gyro_offsets();

    // display results
    hal.console->printf_P(
        PSTR("\nAccel Offsets X:%10.8f \t Y:%10.8f \t Z:%10.8f\n"), accel_offsets.x, accel_offsets.y, accel_offsets.z);
    hal.console->printf_P(
        PSTR("Accel Scale X:%10.8f \t Y:%10.8f \t Z:%10.8f\n"), accel_scale.x, accel_scale.y, accel_scale.z);
    hal.console->printf_P(
        PSTR("Gyro Offsets X:%10.8f \t Y:%10.8f \t Z:%10.8f\n"), gyro_offsets.x, gyro_offsets.y, gyro_offsets.z);
}
