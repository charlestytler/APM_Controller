/*
 * global_data.c - Storage of global data shared across tasks.
 *   Created by Charlie Tytler  2018
 */

#include "global_data.h"

static sensor_measurement_t sensor_measurement;
static vehicle_state_t      estimated_state;
static vehicle_state_t      reference_state;

void write_sensor_barometer_measurement(const barometer_meas_t input)
{
    sensor_measurement.barometer_meas = input;
}

void write_sensor_imu_measurement(const imu_meas_t input)
{
    sensor_measurement.imu_meas = input;
}

void read_sensor_measurement(sensor_measurement_t *output)
{
    *output = sensor_measurement;
}

void write_estimated_state(const vehicle_state_t input)
{
    estimated_state = input;
}

void read_estimated_state(vehicle_state_t *output)
{
    *output = estimated_state;
}

void write_reference_state(const vehicle_state_t input)
{
    reference_state = input;
}

void read_reference_state(vehicle_state_t *output)
{
    *output = reference_state;
}
