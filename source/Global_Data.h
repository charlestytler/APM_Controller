/*
 * global_data.h - Storage of global data shared across tasks.
 *   Created by Charlie Tytler  2018
 */
#ifndef GLOBAL_DATA_H
#define GLOBAL_DATA_H

#include <AP_Math/vector3.h>

// Data type definitions.
typedef struct{
    Vector3f accel;
    Vector3f gyro;
    float baro_alt;
} sensor_measurement_t;

typedef struct{
    Vector3f vel_b_mps;
    Vector3f ang_accel_b_rads;
    Vector3f ang_vel_b_rads;
    Vector3f pos_E_m;
} vehicle_state_t;

// Read/Write access to stored data.
void write_sensor_measurement(const sensor_measurement_t input);
void read_sensor_measurement(sensor_measurement_t *output);
void write_estimated_state(const vehicle_state_t input);
void read_estimated_state(vehicle_state_t *output);
void write_reference_state(const vehicle_state_t input);
void read_reference_state(vehicle_state_t *output);

#endif
