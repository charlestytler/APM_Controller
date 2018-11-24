/*
 * scheduler.c - Library for scheduling tasks for Quadcopter controller to execute each frame.
 * Created by Charlie Tytler  2018
 */

#include "scheduler.h"

// Includes for scheduled functions
#include "communication.h"
#include "imu_fast_loop.h"
#include "sensor_read.h"

#include <stdint.h>
#include <AP_HAL.h>
#include <AP_HAL_AVR.h>
#include <AP_Scheduler.h>


#define MAIN_LOOP_RATE 100
#define MAIN_LOOP_SECONDS 0.01
#define MAIN_LOOP_MICROS 10000


static AP_Scheduler scheduler;


void scheduler_init()
{
    /*
      scheduler table - all regular tasks are listed here, along with how
      often they should be called (in 10ms units) and the maximum time
      they are expected to take (in microseconds)
     */
    static const AP_Scheduler::Task scheduler_tasks[] PROGMEM
        = {{read_baro_alt, 2, 1000}, {comm_one_hz_downlink, 100, 1000}};

    scheduler.init(&scheduler_tasks[0], sizeof(scheduler_tasks) / sizeof(scheduler_tasks[0]));
}

void scheduler_execute()
{
    // Hardware Abstraction Layer (HAL) for APM Board
    const AP_HAL::HAL &hal = AP_HAL_BOARD_DRIVER;

    // First, execute the fast loop
    uint32_t frame_start_time_usec;
    frame_start_time_usec = imu_fast_loop_exectue();

    // Tell the scheduler one tick has passed
    scheduler.tick();

    // run all the tasks that are due to run. Note that we only
    // have to call this once per loop, as the tasks are scheduled
    // in multiples of the main loop tick. So if they don't run on
    // the first call to the scheduler they won't run on a later
    // call until scheduler.tick() is called again
    uint32_t time_available = (frame_start_time_usec + MAIN_LOOP_MICROS) - hal.scheduler->micros();
    scheduler.run(time_available);
}
