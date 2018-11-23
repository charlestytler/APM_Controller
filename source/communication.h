/*
 * communication.h - Library for uplink (to vehicle) and downlink (from vehcile) between vehicle and ground station.
 * Created by Charlie Tytler  2018
 */
#ifndef COMMUNICATION_H
#define COMMUNICATION_H

//
// Configures initial settings for UART communications from Hardware Abstraction Layer (HAL).
//
// [input] hal: Hardware Abstraction Layer class instance.
bool establish_communcunication_link(AP_HAL::HAL hal);

//
// Scheduled task which downlinks logging data at 1 Hz.
//
void comm_one_hz_downlink(void);

#endif
