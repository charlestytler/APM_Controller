/*
 * state_estimation.cpp - Library to estimate quadcopter state using sensor measurement data read from global_data.
 * Created by Charlie Tytler  2018
 */

#include "state_estimation.h"

#include "global_data.h"
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


void update_euler_angle_estimate()
{
    // Read from global_data.
    vehicle_state_t estimated_state;
    read_estimated_state(&estimated_state);
    sensor_measurement_t sensor_measurement;
    read_sensor_measurement(&sensor_measurement);

    // Update estimated_state.
    const float    delta_time = (sensor_measurement.imu_meas.timestamp_usec - estimated_state.timestamp_usec) * 1E-6F;
    vector_t       delta_euler_angle_rad;
    const vector_t euler_angle_dot
        = calculate_xdot(estimated_state.euler_angle_rad, sensor_measurement.imu_meas.gyro_rads);
    scalar_multiply(delta_time, &euler_angle_dot, &delta_euler_angle_rad);
    estimated_state.euler_angle_rad = vector_add(&estimated_state.euler_angle_rad, &delta_euler_angle_rad);
    estimated_state.timestamp_usec  = sensor_measurement.imu_meas.timestamp_usec;

    // Write to global_data.
    write_estimated_state(estimated_state);
}


/*
# Derivative function
def xdot(x,u):
        x_dot = [0,0,0]
            #   phidot = p + (q*sin(phi) + r*cos(phi))*tan(theta)
            x_dot[0] = u[0] + (u[1]*sin(x[0]) + u[2]*cos(x[0]))*tan(x[1])
        #   thetadot = q*cos(phi) - r*sin(phi)
        x_dot[1] = u[1]*cos(x[0]) - u[2]*sin(x[0])
        #   psidot = (q*sin(phi) + r*cos(phi))*sec(theta)
        x_dot[2] = (u[1]*sin(x[0]) + u[2]*cos(x[0]))*sec(x[1])
        return x_dot
# Initial condition setup
        eulerAngles = [0,0,0]      #Assume start up with phi = theta = psi = 0 radians
        deltaT = 0.01              #Sampling time (sec)
# While loop
    while True:
        omegaRead = gyros.getAngularVelocity()
        eulerAngles += xdot(eulerAngles,omegaRead) * deltaT


def accelLevel(accelRead):
        # Takes accelerometer specific force reading when stationary and estimates
        # phi and theta from the gravity vector
        fx_b = accelRead[0]
            fy_b = accelRead[1]
                fz_b = accelRead[2]
                    phi_est = arctan2(-fy_b,-fz_b)
        theta_est = arctan2(fx_b, sqrt(fy_b*fy_b + fz_b*fz_b))

        return phi_est, theta_est
*/
