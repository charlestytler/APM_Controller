/*
 * state_estimation.cpp - Library to estimate quadcopter state using sensor measurement data read from global_data.
 * Created by Charlie Tytler  2018
 */

#include "state_estimation.h"

#include "linear_algebra.h"
#include <math.h>


static vector_t calculate_xdot(vector_t angle, vector_t angular_rate)
{
    vector_t euler_angle_dot; // phi, theta, psi
    float    p_b       = angular_rate.x;
    float    q_b       = angular_rate.y;
    float    r_b       = angular_rate.z;
    float    sin_phi   = sin(angle.x);
    float    cos_phi   = cos(angle.x);
    float    sin_theta = sin(angle.y);
    float    cos_theta = cos(angle.y);
    // Protect against divide by zero in below equations.
    if (cos_theta < 1E-6)
    {
        cos_theta = 1E-6;
    }

    // phidot = p + (q*sin(phi) + r*cos(phi)) * tan(theta)
    euler_angle_dot.x = p_b + (q_b * sin_phi + r_b * cos_phi * sin_theta / cos_theta);
    // thetadot = q*cos(phi) - r*sin(phi)
    euler_angle_dot.y = q_b * cos_phi - r_b * sin_phi;
    // psidot = (q*sin(phi) + r*cos(phi)) * sec(theta)
    euler_angle_dot.z = (q_b * sin_phi + r_b * cos_phi) / cos_theta;

    return euler_angle_dot;
}


void state_estimation_init()
{
    vehicle_state_t estimated_state;
    estimated_state.vel_b_mps.x       = 0.0F;
    estimated_state.vel_b_mps.y       = 0.0F;
    estimated_state.vel_b_mps.z       = 0.0F;
    estimated_state.ang_vel_b_rads.x  = 0.0F;
    estimated_state.ang_vel_b_rads.y  = 0.0F;
    estimated_state.ang_vel_b_rads.z  = 0.0F;
    estimated_state.euler_angle_rad.x = 0.0F;
    estimated_state.euler_angle_rad.y = 0.0F;
    estimated_state.euler_angle_rad.z = 0.0F;
    estimated_state.pos_E_m.x         = 0.0F;
    estimated_state.pos_E_m.y         = 0.0F;
    estimated_state.pos_E_m.z         = 0.0F;

    write_estimated_state(estimated_state);
}


void state_estimation_execute()
{
    // Read from global_data.
    vehicle_state_t estimated_state;
    read_estimated_state(&estimated_state);
    sensor_measurement_t sensor_measurement;
    read_sensor_measurement(&sensor_measurement);

    // Update state estimate.
    propagate_state_estimate(&sensor_measurement.imu_meas, &estimated_state);
    measurement_update(&sensor_measurement, &estimated_state);

    // Store timestamp for estimated_state.
    estimated_state.timestamp_usec = sensor_measurement.imu_meas.timestamp_usec;

    // Write to global_data.
    write_estimated_state(estimated_state);
}

void propagate_state_estimate(const imu_meas_t *imu_meas, vehicle_state_t *estimated_state)
{
    const float delta_time = (imu_meas->timestamp_usec - estimated_state->timestamp_usec) * 1E-6F;

    // Euler angle update from gyro.
    vector_t       delta_euler_angle_rad;
    const vector_t euler_angle_dot = calculate_xdot(estimated_state->euler_angle_rad, imu_meas->gyro_rads);
    scalar_multiply(delta_time, &euler_angle_dot, &delta_euler_angle_rad);
    estimated_state->euler_angle_rad = vector_add(&estimated_state->euler_angle_rad, &delta_euler_angle_rad);
}

void measurement_update(const sensor_measurement_t *sensor_measurement, vehicle_state_t *estimated_state)
{
    // Euler angle measurement from accelerometer.
    const float accel_y_sq = sensor_measurement->imu_meas.accel_mpss.y * sensor_measurement->imu_meas.accel_mpss.y;
    const float accel_z_sq = sensor_measurement->imu_meas.accel_mpss.z * sensor_measurement->imu_meas.accel_mpss.z;

    // Apply complimentary filter update to euler angle estimates if acceleration measurement magnitude is near 9.8mpss.
    const float accel_magnitude_sq
        = sensor_measurement->imu_meas.accel_mpss.x * sensor_measurement->imu_meas.accel_mpss.x + accel_y_sq
          + accel_z_sq;
    if (90.0F < accel_magnitude_sq && accel_magnitude_sq < 110.0F)
    {
        float phi_est = atan2f(-sensor_measurement->imu_meas.accel_mpss.y, -sensor_measurement->imu_meas.accel_mpss.z);
        float theta_est = atan2f(sensor_measurement->imu_meas.accel_mpss.x, sqrtf(accel_y_sq + accel_z_sq));

        estimated_state->euler_angle_rad.x = 0.99F * estimated_state->euler_angle_rad.x + 0.01F * phi_est;
        estimated_state->euler_angle_rad.y = 0.99F * estimated_state->euler_angle_rad.y + 0.01F * theta_est;
    }
}
