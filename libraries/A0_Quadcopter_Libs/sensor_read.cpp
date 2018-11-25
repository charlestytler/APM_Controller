/*
 * sensor_read.cpp - Library for updating sensor values from APM hardware
 * Created by Charlie Tytler  2018
 */

#include "sensor_read.h"

// Read/write capability to global data.
#include "global_data.h"

// Sensor drivers libraries
#include <AP_Baro.h>
#include <AP_GPS.h>
#include <AP_Compass.h>
#include <AP_Declination.h>
#include <AP_HAL.h>
#include <AP_HAL_AVR.h>
#include <stdint.h>

// Initialize sensor classes
static AP_GPS             gps;
static AP_Baro_MS5611     barometer(&AP_Baro_MS5611::spi);
static AP_Compass_HMC5843 compass;

// TODO: Battery
// static AP_BattMonitor battery;


bool sensor_init_and_calibrate()
{
    barometer.init();
    // TODO: gps.init(&DataFlash);
    // TODO: battery.init();
    // TODO: init_compass();

    barometer.calibrate();


    // TODO: Return fault flag if calibrations are unsuccessful
    return true;
}

void read_baro_alt()
{
    // Hardware Abstraction Layer (HAL) for APM Board
    const AP_HAL::HAL &hal = AP_HAL_BOARD_DRIVER;

    barometer.read();

    barometer_meas_t barometer_meas;
    barometer_meas.altitude_m     = barometer.get_altitude();
    barometer_meas.timestamp_usec = hal.scheduler->micros();

    write_sensor_barometer_measurement(barometer_meas);

    // TODO: fault if (barometer.healthy())
}
