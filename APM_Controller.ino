#include <stdarg.h>
#include <AP_Common.h>
#include <AP_Progmem.h>
#include <AP_HAL.h>
#include <AP_HAL_AVR.h>
#include <AP_Math.h>
#include <AP_Param.h>
#include <StorageManager.h>

#define FRAME_TIME 10000    //microseconds
#define MIN_TASK_TIME 2000  //microseconds

// Identify which board type for Hardware Abstraction Layer (HAL)
const AP_HAL::HAL& hal = AP_HAL_BOARD_DRIVER;


void setup(void){
  //Set baud rate to 57600 in order to transmit over SIK Radio
  hal.uartA->begin(map_baudrate(57600));
  
  // Calibrate accel
  // Calibrate gyro
  
  // Initialize task order for execution frame
  
}

void loop(void){
  
  // Get start time of frame
  uint32_t frame_start_time = hal.scheduler->micros();
  
  // Iterate through tasks while time is available
  int task_count = 0;
  
  do {
    // Check if at least MIN_TASK_TIME msec are left in frame to complete next task
    if ( FRAME_TIME - (hal.scheduler->micros() - frame_start_time) >= MIN_TASK_TIME ) {
      // Execute task
      task_count = execute_task(task_count, frame_start_time);
    }
  }while (task_count != 0);
  
  // Wait until FRAME_TIME is reached
  while (hal.scheduler->micros() - frame_start_time < FRAME_TIME){
    hal.scheduler->delay(1);
  }
  
}


int execute_task(int task_count, uint32_t frame_start_time){
  
  switch (task_count) {
    
    case 0:
      // Control commands to motors
      break;
    
    case 1:
      // Read sensors
      break;
    
    case 2:
      // Propagate state
      break;
    
    case 3:
      // Complimentary Filter
      break;
    
    case 4:
      // Read inputs from receiver
      break;
    
    case 5:
      // Calculate control commands
      break;
    
    case 6:
      // Transmit status data
      transmit_serial(frame_start_time);
      return 0;    
  }
  
  return ++task_count;
}


void transmit_serial(uint32_t frame_start_time){
  // Print out how much time was used in this frame
  hal.console->printf_P(PSTR("%d usec\n"), (int) hal.scheduler->micros() - frame_start_time);
}

AP_HAL_MAIN();
