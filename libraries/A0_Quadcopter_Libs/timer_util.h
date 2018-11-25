/*
 * timer_util.cpp - A debug utility to time a procedure on target.
 */

#ifndef TIMER_UTIL_H
#define TIMER_UTIL_H

#include <stdint.h>

//
// Returns start time in microseconds to provide to `print_delta_time_micros` after the process to be timed has
// completed.
//
uint32_t get_start_time_micros();

//
// Returns start time in milliseconds to provide to `print_delta_time_micros` after the process to be timed has
// completed.
//
uint32_t get_start_time_millis();

//
// Prints to console the delta time in microseconds from the provided start_time.
//
void print_delta_time_micros(uint32_t start_time);

//
// Prints to console the delta time in milliseconds from the provided start_time.
//
void print_delta_time_millis(uint32_t start_time);

#endif
