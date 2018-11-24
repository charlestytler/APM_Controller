/*
 * communication.c - Library for uplink (to vehicle) and downlink (from vehcile) between vehicle and ground station.
 * Created by Charlie Tytler  2018
 */

#include "communication.h"

#include "global_data.h"

#include <AP_Common.h>
#include <AP_HAL.h>
#include <AP_HAL_AVR.h>
#include <stdint.h>

bool establish_communication_link()
{
    // Hardware Abstraction Layer (HAL) for APM Board
    const AP_HAL::HAL &hal = AP_HAL_BOARD_DRIVER;

    /*
    if (!hal.gpio->usb_connected()) {
        // USB is not connected, this means UART0 may be a Xbee, with
        // its darned bricking problem. We can't write to it for at
        // least one second after powering up. Simplest solution for
        // now is to delay for 1 second. Something more elegant may be
        // added later
        hal.scheduler->delay(1000);
    }
    */

    // Set baud rate to 57600 in order to transmit over SIK Radio
    hal.uartA->begin(map_baudrate(57600));

    // Initialize serial port for GPS receiver
    // hal.uartB->begin(38400, 256, 16);

    /*
      run the timer a bit slower on APM2 to reduce the interrupt load
      on the CPU
     */
    hal.scheduler->set_timer_speed(500);


    // we don't want writes to the serial port to cause us to pause
    // mid-flight, so set the serial ports non-blocking once we are
    // ready to fly
    hal.uartA->set_blocking_writes(false);
    // hal.uartB->set_blocking_writes(false);

    hal.console->print_P(PSTR("\nInitial Setup Complete\n"));

    // TODO: return false if not successful.
    return true;
}


void comm_one_hz_downlink(void)
{
    // Hardware Abstraction Layer (HAL) for APM Board
    const AP_HAL::HAL &hal = AP_HAL_BOARD_DRIVER;

    sensor_measurement_t sensor_measurement;
    read_sensor_measurement(&sensor_measurement);

    /*
    // Planned serialized downlink.
    uint8_t buffer[100];
    if (sizeof(buffer) > sizeof(sensor_measurement))
    {
        memcpy(&buffer, &sensor_measurement, sizeof(sensor_measurement));
    }

    hal.console->print_P(PSTR("BEG"));
    for (uint16_t i = 0; i < sizeof(buffer); i++)
    {
        hal.console->write(buffer[i]);
    }
    hal.console->print_P(PSTR("END\n"));
    */

    // Output as a string for debugging
    hal.console->printf("IMU ax:%f ay:%f az:%f t:%f  Gyro wx:%f wy:%f wz:%f  Baro alt:%f t:%f\n",
                        sensor_measurement.imu_meas.accel_mps.x,
                        sensor_measurement.imu_meas.accel_mps.y,
                        sensor_measurement.imu_meas.accel_mps.z,
                        (float) sensor_measurement.imu_meas.timestamp_usec / 1.0E6,
                        sensor_measurement.imu_meas.gyro_rads.x,
                        sensor_measurement.imu_meas.gyro_rads.y,
                        sensor_measurement.imu_meas.gyro_rads.z,
                        sensor_measurement.barometer_meas.altitude_m,
                        (float) sensor_measurement.barometer_meas.timestamp_usec / 1.0E6);
}
