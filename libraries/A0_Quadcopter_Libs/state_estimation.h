/*
 * state_estimation.cpp - Library to estimate quadcopter state using sensor measurement data read from global_data.
 * Created by Charlie Tytler  2018
 */

#ifndef STATE_ESTIMATION_H
#define STATE_ESTIMATION_H


void state_estimation_init();

void update_euler_angle_estimate();


#endif
