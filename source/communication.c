/*
 * communication.c - Library for uplink (to vehicle) and downlink (from vehcile) between vehicle and ground station.
 * Created by Charlie Tytler  2018
 */

#include "communication.h"


bool establish_communcunication_link(AP_HAL::HAL hal)
{
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

    //Set baud rate to 57600 in order to transmit over SIK Radio
    hal.uartA->begin(map_baudrate(57600));

    //Initialize serial port for GPS receiver
    //hal.uartB->begin(38400, 256, 16);

    /*
      run the timer a bit slower on APM2 to reduce the interrupt load
      on the CPU
     */
    hal.scheduler->set_timer_speed(500);


    // we don't want writes to the serial port to cause us to pause
    // mid-flight, so set the serial ports non-blocking once we are
    // ready to fly
    hal.uartA->set_blocking_writes(false);
    //hal.uartB->set_blocking_writes(false);

    hal.console->print_P(PSTR("\nInitial Setup Complete"));
}


void comm_one_hz_downlink(void)
{
    hal.console->printf("one_hz: t=%lu\n", (unsigned long)hal.scheduler->millis());
    hal.console->printf("accel = %4.2f %4.2f %4.2f\n", sensor_readings.accel.x, sensor_readings.accel.y, sensor_readings.accel.z);
    hal.console->printf("gyro = %4.2f %4.2f %4.2f\n", sensor_readings.gyro.x, sensor_readings.gyro.y, sensor_readings.gyro.z);
    hal.console->printf("baro = %f\n", sensor_readings.baro_alt);
}
