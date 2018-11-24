

//Ardupilot Libraries
// Dependencies for HAL and Sensor libraries
#include <AP_Progmem.h>
#include <AP_Math.h>
#include <AP_Common.h>
#include <AP_Notify.h>
#include <AP_Param.h>
#include <Filter.h>
#include <GCS_MAVLink.h>
#include <DataFlash.h>
#include <AP_Vehicle.h>
#include <AP_AHRS.h>
#include <AP_Airspeed.h>
#include <AP_Mission.h>
#include <AP_ADC.h>
#include <AP_ADC_AnalogSource.h>
#include <AP_Terrain.h>
// APM2 Hardware Abstraction Layer (HAL) libraries
#include <AP_HAL.h>
#include <AP_HAL_AVR.h>
#include <AP_Scheduler.h>
#include <StorageManager.h>
// Sensor drivers libraries
#include <AP_InertialSensor.h>
#include <AP_Baro.h>
#include <AP_GPS.h>
#include <AP_Compass.h>
#include <AP_Declination.h>



// Quadcopter Libraries
#include <communication.h>
#include <controller.h>
#include <global_data.h>
#include <imu_fast_loop.h>
#include <motor_command.h>
#include <sensor_read.h>
#include <task_scheduler.h>


// Hardware Abstraction Layer (HAL) for APM Board
const AP_HAL::HAL &hal = AP_HAL_BOARD_DRIVER;


void setup(void)
{
    imu_fast_loop_init();
    sensor_init_and_calibrate();
    motor_command_init();
    scheduler_init();
    establish_communication_link();
}

void loop()
{
    scheduler_execute();
}

AP_HAL_MAIN();
