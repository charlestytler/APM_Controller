
## Communication Downlink
### Comparison of bytes buffer vs. string print timing

Logging with a buffer of bytes:

```
    uint32_t start_time = hal.scheduler->micros();

    // Planned serialized downlink.
    uint8_t buffer[100];
    if (sizeof(buffer) > sizeof(sensor_measurement))
    {
        memcpy(&buffer, &sensor_measurement, sizeof(sensor_measurement));
    }

    hal.console->print_P(PSTR("BEG"));
    for (uint16_t i = 0; i < sizeof(buffer); i++)
    {
        hal.console->write(buffer[i]);
    }
    hal.console->print_P(PSTR("END\n"));

    uint32_t end_time = hal.scheduler->micros();
```

Runs from start_time to end_time in 0.7-0.9 msec



Logging with a Formatted String:

```
    uint32_t start_time = hal.scheduler->millis();

    // Output as a string for debugging
    hal.console->printf("IMU ax:%f ay:%f az:%f t:%f  Gyro wx:%f wy:%f wz:%f  Baro alt:%f t:%f\n",
                        sensor_measurement.imu_meas.accel_mps.x,
                        sensor_measurement.imu_meas.accel_mps.y,
                        sensor_measurement.imu_meas.accel_mps.z,
                        (float) sensor_measurement.imu_meas.timestamp_usec / 1.0E6,
                        sensor_measurement.imu_meas.gyro_rads.x,
                        sensor_measurement.imu_meas.gyro_rads.y,
                        sensor_measurement.imu_meas.gyro_rads.z,
                        sensor_measurement.barometer_meas.altitude_m,
                        (float) sensor_measurement.barometer_meas.timestamp_usec / 1.0E6);


    uint32_t end_time = hal.scheduler->millis();
```

Runs from start_time to end_time in 10-12 msec


Logging with String without labels:
```
    uint32_t start_time = hal.scheduler->millis();

    // Output as a string for debugging
    hal.console->printf("%f %f %f %f %f %f %f %f %f\n",
                        sensor_measurement.imu_meas.accel_mps.x,
                        sensor_measurement.imu_meas.accel_mps.y,
                        sensor_measurement.imu_meas.accel_mps.z,
                        (float) sensor_measurement.imu_meas.timestamp_usec / 1.0E6,
                        sensor_measurement.imu_meas.gyro_rads.x,
                        sensor_measurement.imu_meas.gyro_rads.y,
                        sensor_measurement.imu_meas.gyro_rads.z,
                        sensor_measurement.barometer_meas.altitude_m,
                        (float) sensor_measurement.barometer_meas.timestamp_usec / 1.0E6);


    uint32_t end_time = hal.scheduler->millis();
```

Runs from start_time to end_time in 10-12 msec

Logging a String without any substitution:
```
	uint32_t start_time = hal.scheduler->micros();

    hal.console->printf("Here is a string that is at least 36 bytes\n");

    uint32_t end_time = hal.scheduler->micros();
```

Runs from start_time to end_time in 0.3-0.4 msec 