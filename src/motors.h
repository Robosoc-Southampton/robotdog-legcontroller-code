#ifndef MOTORS_H
#define MOTORS_H

#include <stdint.h>

#define MOTOR_PORTS 6

enum motor_type {
    NONE      = 0b00,
    SERVO     = 0b01,
    BRUSHLESS = 0b10,
    CUSTOM    = 0b11
};

struct motor {
    enum motor_type type;
    uint8_t current_limit;
    bool overcurrent_flag;
    uint16_t target_pos, target_vel, target_acc;
    uint16_t actual_pos;
};

/**
 * Array of all the motors. All the motors can be controlled
 * through this.
 */
extern struct motor motors[MOTOR_PORTS];

/**
 * Initialises the motors.
 */
void init_motors();

/**
 * Updates all the motors.
 */
void update_motors(uint8_t millis);

#endif /* MOTORS_H */
