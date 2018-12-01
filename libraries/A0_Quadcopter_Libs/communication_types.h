/*
 * communication_types.h - Collection of type definitions for uplinking and downlinking information.
 * Created by Charlie Tytler  2018
 */
#ifndef COMMUNICATION_TYPES_H
#define COMMUNICATION_TYPES_H

#include "global_data.h"

typedef struct
{
    sensor_measurement_t sensor_measurement;
    vehicle_state_t      estimated_state;
    uint32_t             downlink_timestamp_usec;
} downlink_message_t;

#endif
