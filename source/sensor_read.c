/*
 * sensor_read.c - Library for updating sensor values from APM hardware
 * Created by Charlie Tytler  2018
 */

#include "sensor_read.h"

// Sensor drivers libraries
#include <AP_Baro.h>
#include <AP_GPS.h>
#include <AP_Compass.h>
#include <AP_Declination.h>
#include <AP_InertialSensor.h>

// Initialize sensor classes
static AP_GPS             gps;
static AP_Baro_MS5611     barometer(&AP_Baro_MS5611::spi);
static AP_Compass_HMC5843 compass;

//TODO: Battery
//static AP_BattMonitor battery;


bool sensor_init_and_calibrate()
{

    barometer.init();
    //TODO: gps.init(&DataFlash);
    //TODO: battery.init();
    //TODO: init_compass();

    barometer.calibrate();

    //TODO: Return fault flag if calibrations are unsuccessful
    return true;
}

bool read_baro_alt(const AP_Baro_MS5611 barometer, float *altitude_m)
{
    barometer.read();
    altitude_m = barometer.get_altitude();

    if (barometer.healthy())
    {
        return true;
    }
    else
    {
        return false;
    }
}


bool read_imu(const AP_InertialSensor imu, accel_t *accel_meas, gyro_t *gyro_meas)
{
    imu.update();
    accel_meas = imu.get_accel();
    gyro_meas = imu.get_gyro();

    //TODO: Return fault flag if imu read is unsuccessful
    return true;
}
