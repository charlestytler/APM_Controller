/*
 * global_data.h - Storage of global data shared across tasks.
 *   Created by Charlie Tytler  2018
 */
#ifndef GLOBAL_DATA_H
#define GLOBAL_DATA_H

#include <stdint.h>

// Data type definitions.
typedef struct
{
    float x;
    float y;
    float z;
} vector_t;

typedef struct
{
    vector_t accel_mps;
    vector_t gyro_rads;
    uint32_t timestamp_usec;
} imu_meas_t;

typedef struct
{
    float    altitude_m;
    uint32_t timestamp_usec;
} barometer_meas_t;

typedef struct
{
    imu_meas_t       imu_meas;
    barometer_meas_t barometer_meas;
} sensor_measurement_t;

typedef struct
{
    vector_t vel_b_mps;
    vector_t ang_accel_b_rads;
    vector_t ang_vel_b_rads;
    vector_t pos_E_m;
} vehicle_state_t;

// Read/Write access to stored data (writes *to* global_data, and reads *from* global_data).
void write_sensor_barometer_measurement(const barometer_meas_t input);
void write_sensor_imu_measurement(const imu_meas_t input);
void read_sensor_measurement(sensor_measurement_t *output);
void write_estimated_state(const vehicle_state_t input);
void read_estimated_state(vehicle_state_t *output);
void write_reference_state(const vehicle_state_t input);
void read_reference_state(vehicle_state_t *output);

#endif
