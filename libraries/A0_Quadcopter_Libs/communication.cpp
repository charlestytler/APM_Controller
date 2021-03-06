/*
 * communication.cpp - Library for uplink (to vehicle) and downlink (from vehcile) between vehicle and ground station.
 * Created by Charlie Tytler  2018
 */

#include "communication.h"

#include "communication_types.h"
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


void comm_logging_downlink(void)
{
    // Hardware Abstraction Layer (HAL) for APM Board
    const AP_HAL::HAL &hal = AP_HAL_BOARD_DRIVER;

    sensor_measurement_t sensor_measurement;
    read_sensor_measurement(&sensor_measurement);
    vehicle_state_t estimated_state;
    read_estimated_state(&estimated_state);

    downlink_message_t downlink_msg;
    downlink_msg.sensor_measurement      = sensor_measurement;
    downlink_msg.estimated_state         = estimated_state;
    downlink_msg.downlink_timestamp_usec = hal.scheduler->micros();

    // Serialized downlink.
    // Can support 128 bytes total, running at 50 Hz performs well with 100 bytes over USB,
    // but < 50 bytes reliably for SiK Radio.
    uint8_t buffer[100];
    if (sizeof(buffer) > sizeof(downlink_msg))
    {
        memcpy(&buffer, &downlink_msg, sizeof(downlink_msg));
    }
    else
    {
        hal.console->print("Buffer is too small\n");
    }


    hal.console->print_P(PSTR("BEG"));
    for (uint16_t i = 0; i < sizeof(buffer); i++)
    {
        hal.console->write(buffer[i]);
    }
    hal.console->print_P(PSTR("END\n"));
}
