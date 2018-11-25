/*
 * timer_util.cpp - A debug utility to time a procedure on target.
 * Created by Charlie Tytler  2018
 */

#include <AP_HAL.h>
#include <AP_HAL_AVR.h>

uint32_t get_start_time_micros()
{
    const AP_HAL::HAL &hal        = AP_HAL_BOARD_DRIVER;
    uint32_t           start_time = hal.scheduler->micros();
    return start_time;
}

uint32_t get_start_time_millis()
{
    const AP_HAL::HAL &hal        = AP_HAL_BOARD_DRIVER;
    uint32_t           start_time = hal.scheduler->millis();
    return start_time;
}

void print_delta_time_micros(uint32_t start_time)
{
    const AP_HAL::HAL &hal      = AP_HAL_BOARD_DRIVER;
    uint32_t           end_time = hal.scheduler->micros();

    hal.console->printf("\nStart: %lu End: %lu Delta us: %lu\n",
                        (unsigned long) start_time,
                        (unsigned long) end_time,
                        (unsigned long) (end_time - start_time));
}

void print_delta_time_millis(uint32_t start_time)
{
    const AP_HAL::HAL &hal      = AP_HAL_BOARD_DRIVER;
    uint32_t           end_time = hal.scheduler->millis();

    hal.console->printf("\nStart: %lu End: %lu Delta ms: %lu\n",
                        (unsigned long) start_time,
                        (unsigned long) end_time,
                        (unsigned long) (end_time - start_time));
}
