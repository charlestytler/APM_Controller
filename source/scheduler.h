/*
 * scheduler.h - Library for scheduling tasks for Quadcopter controller to execute each frame.
 * Created by Charlie Tytler  2018
 */
#ifndef SCHEDULER_H
#define SCHEDULER_H

//
// Initializes schedule of tasks to be run and their frequency.
//
void scheduler_init();

//
// Executes one 10ms frame, calling all functions expected to run in the frame.
//
void scheduler_execute();

#endif
