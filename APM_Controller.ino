#include <stdarg.h>
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


# define MAIN_LOOP_RATE    100
# define MAIN_LOOP_SECONDS 0.01
# define MAIN_LOOP_MICROS  10000


// Identify which board type for Hardware Abstraction Layer (HAL)
const AP_HAL::HAL& hal = AP_HAL_BOARD_DRIVER;


// Global parameters are all contained within the 'g' class.
//
//static Parameters g;

// main loop scheduler
static AP_Scheduler scheduler;


// main loop sampling rate
static const AP_InertialSensor::Sample_rate ins_sample_rate = AP_InertialSensor::RATE_100HZ;



// Identify sensors
//static AP_GPS  gps;
static AP_Baro_MS5611 barometer(&AP_Baro_MS5611::spi);
//#if CONFIG_COMPASS == HAL_COMPASS_HMC5843
//static AP_Compass_HMC5843 compass;
//#endif
AP_InertialSensor ins;

// Sensor fusion
//AP_AHRS_NavEKF ahrs(ins, barometer, gps);
//static AP_InertialNav_NavEKF inertial_nav(ahrs, barometer, gps_glitch, baro_glitch);


//static int32_t baro_alt;            // barometer altitude in cm above home
static struct {
  float baro_alt;
  Vector3f accel;
  Vector3f gyro;
} sensor_readings;

// Integration time (in seconds) for the gyros (DCM algorithm)
// Updated with the fast loop
static float G_Dt = 0.02;


// TODO: Define motors

// MAYBE: Battery
//static AP_BattMonitor battery;


//AC_AttitudeControl_Heli attitude_control(ahrs, aparm, motors, g.p_stabilize_roll, g.p_stabilize_pitch, g.p_stabilize_yaw,
//                        g.pid_rate_roll, g.pid_rate_pitch, g.pid_rate_yaw);
//static AC_WPNav wp_nav(inertial_nav, ahrs, pos_control);
//static AC_Circle circle_nav(inertial_nav, ahrs, pos_control);



// System Timers
// --------------
// Time in microseconds of main control loop
static uint32_t fast_loopTimer;
// Counter of main loop executions.  Used for performance monitoring and failsafe processing
static uint16_t mainLoop_count;
static uint16_t ins_counter;

/*
  scheduler table - all regular tasks are listed here, along with how
  often they should be called (in 10ms units) and the maximum time
  they are expected to take (in microseconds)
 */
static const AP_Scheduler::Task scheduler_tasks[] PROGMEM = {
    { read_ins,                1,   1000},
    { read_baro_alt,           2,   1000},
    { one_hz_print,          100,   1000},
    { five_second_call,      500,   1800}
};



//static const AP_Scheduler::Task scheduler_tasks[] PROGMEM = {
//    { rc_loop,               1,     100 },
//    { throttle_loop,         2,     450 },
//    { update_GPS,            2,     900 },
//    { update_batt_compass,  10,     720 },
//    { arm_motors_check,     10,      10 },
//    { auto_trim,            10,     140 },
//    { barometer_accumulate,  2,     250 },
//    { update_altitude,      10,    1000 },
//    { compass_accumulate,    2,     420 },
//    { check_dynamic_flight,  2,     100 },
//    { run_nav_updates,       4,     800 },
//    { update_thr_cruise,     1,      50 },
//    { three_hz_loop,        33,      90 },
//    { update_notify,         2,     100 },
//    { one_hz_loop,         100,     420 },
//    { ekf_dcm_check,        10,      20 },
//    { crash_check,          10,      20 },
//    { gcs_check_input,	     2,     550 },
//    { gcs_send_heartbeat,  100,     150 },
//    { gcs_send_deferred,     2,     720 },
//    { gcs_data_stream_send,  2,     950 },
//    { update_mount,          2,     450 },
//    { ten_hz_logging_loop,  10,     300 },
//    { fifty_hz_logging_loop, 2,     220 },
//    { perf_update,        1000,     200 },
//    { read_receiver_rssi,   10,      50 },
//#if FRSKY_TELEM_ENABLED == ENABLED
//    { telemetry_send,       20,     100 }
//};


void setup(void){

  init_quadcopter();

  // initialise the main loop scheduler
  scheduler.init(&scheduler_tasks[0], sizeof(scheduler_tasks)/sizeof(scheduler_tasks[0]));

}

void loop(){

  // wait for an INS sample
  ins.wait_for_sample();

  // Get start time of frame
  uint32_t timer = hal.scheduler->micros();

  // used by Control Loops
  G_Dt                    = (float)(timer - fast_loopTimer) / 1000000.f;
  fast_loopTimer          = timer;


  // for mainloop failure monitoring
  mainLoop_count++;


  // Execute the fast loop
  // ---------------------
  fast_loop();

  // tell the scheduler one tick has passed
  scheduler.tick();

  // run all the tasks that are due to run. Note that we only
  // have to call this once per loop, as the tasks are scheduled
  // in multiples of the main loop tick. So if they don't run on
  // the first call to the scheduler they won't run on a later
  // call until scheduler.tick() is called again
  uint32_t time_available = (timer + MAIN_LOOP_MICROS) - hal.scheduler->micros();
  scheduler.run(time_available);

}


// Main loop - 100hz
static void fast_loop()
{
/*
    // IMU DCM Algorithm
    // --------------------
    read_AHRS();

    // run low level rate controllers that only require IMU data
    attitude_control.rate_controller_run();

    update_heli_control_dynamics();

    // write out the servo PWM values
    // ------------------------------
    set_servos_4();

    // Inertial Nav
    // --------------------
    read_inertia();

    // run the attitude controllers
    update_flight_mode();
*/
}


void init_quadcopter(){

  if (!hal.gpio->usb_connected()) {
      // USB is not connected, this means UART0 may be a Xbee, with
      // its darned bricking problem. We can't write to it for at
      // least one second after powering up. Simplest solution for
      // now is to delay for 1 second. Something more elegant may be
      // added later
      hal.scheduler->delay(1000);
  }

  //Set baud rate to 57600 in order to transmit over SIK Radio
  hal.uartA->begin(map_baudrate(57600));

  //Initialize serial port for GPS receiver
  //hal.uartB->begin(38400, 256, 16);

  /*
    run the timer a bit slower on APM2 to reduce the interrupt load
    on the CPU
   */
  hal.scheduler->set_timer_speed(500);

  // initialise battery monitor
  //battery.init();

  barometer.init();

  //init_rc_out();              // sets up motors and output to escs

  // Do GPS init
  //gps.init(&DataFlash);

  //init_compass();

  // initialise attitude and position controllers
  //attitude_control.set_dt(MAIN_LOOP_SECONDS);
  //pos_control.set_dt(MAIN_LOOP_SECONDS);

  // initialise inertial nav
  //inertial_nav.init();

  // read Baro pressure at ground
  //init_barometer(true);
  barometer.calibrate();

  startup_ground();

  // we don't want writes to the serial port to cause us to pause
  // mid-flight, so set the serial ports non-blocking once we are
  // ready to fly
  hal.uartA->set_blocking_writes(false);
  //hal.uartB->set_blocking_writes(false);

  hal.console->print_P(PSTR("\nInitial Setup Complete"));

}


static void startup_ground() {

  // initialise ahrs (may push imu calibration into the mpu6000 if using that device).
  //ahrs.init();

  // initialize INS with sample rate of 100Hz
  ins.init(AP_InertialSensor::COLD_START, AP_InertialSensor::RATE_100HZ);

  // reset ahrs gyro bias
  //if (force_gyro_cal) {
  //    ahrs.reset_gyro_drift();
  //}

}



void read_baro_alt(){

  barometer.read();
  sensor_readings.baro_alt = barometer.get_altitude();

  if (!barometer.healthy()) {
    hal.console->println("Baro not healthy");
  }
}

/*
  update inertial sensor, reading data
 */
void read_ins(void)
{
    ins_counter++;
    ins.update();
    sensor_readings.accel = ins.get_accel();
    sensor_readings.gyro = ins.get_gyro();
}

void one_hz_print(void)
{
    hal.console->printf("one_hz: t=%lu\n", (unsigned long)hal.scheduler->millis());
    hal.console->printf("accel = %4.2f %4.2f %4.2f\n", sensor_readings.accel.x, sensor_readings.accel.y, sensor_readings.accel.z);
    hal.console->printf("gyro = %4.2f %4.2f %4.2f\n", sensor_readings.gyro.x, sensor_readings.gyro.y, sensor_readings.gyro.z);
    hal.console->printf("baro = %f\n", sensor_readings.baro_alt);
}

void five_second_call(void)
{
    hal.console->printf("five_seconds: t=%lu ins_counter=%u mainLoop_count=%u\n", (unsigned long)hal.scheduler->millis(), ins_counter, mainLoop_count);
}


AP_HAL_MAIN();
