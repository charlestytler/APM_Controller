/*
 * communication.h - Library for uplink (to vehicle) and downlink (from vehcile) between vehicle and ground station.
 * Created by Charlie Tytler  2018
 */
#ifndef COMMUNICATION_H
#define COMMUNICATION_H

#include <stdbool.h>

//
// Configures initial settings for UART communications from Hardware Abstraction Layer (HAL).
//
// [input] hal: Hardware Abstraction Layer class instance.
bool establish_communication_link();

//
// Scheduled task which downlinks logging data at 1 Hz.
//
void comm_one_hz_downlink(void);

#endif
