#include <stdarg.h>
#include <scheduler.h>
#include <imu_fast_loop.h>
#include <sensor_read.h>
#include <motor_command.h>
#include <communication.h>

void setup(void)
{
    imu_fast_loop_init();
    sensor_initialize_and_calibrarte();
    motor_command_init();
    scheduler_init();
    establish_communication_link();
}

void loop()
{
    scheduler_execute();
}

AP_HAL_MAIN();
