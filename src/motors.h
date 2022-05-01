#ifndef MOTORS_H
#define MOTORS_H

#include <stdint.h>

#define MOTOR_PORTS 3

enum motor_type {
    NONE      = 0b00,
    SERVO     = 0b01,
    BRUSHLESS = 0b10,
    CUSTOM    = 0b11
};

struct motor {
    const enum motor_type type;
    const uint16_t precision; // the number of divisions in one revolution
    int16_t target_pos;
    int16_t actual_pos;
};

/**
 * Array of all the motors. All the motors can be controlled
 * through this.
 */
extern struct motor motors[MOTOR_PORTS];

/**
 * Writing a `1` to a bit in this variable will calibrate the corresponding
 * motor. The bit will be automatically cleared once calibration is complete.
 */
extern uint8_t motor_calibrate_flags;

/**
 * Each motor can be enabled/disabled by writing a `1` or `0` respectively
 * to the corresponding bit in this variable.
 */
extern uint8_t motor_enabled_flags;

/**
 * Initialises the motors.
 */
void init_motors();

/**
 * Updates all the motors.
 */
void update_motors(uint8_t millis);

#endif /* MOTORS_H */
