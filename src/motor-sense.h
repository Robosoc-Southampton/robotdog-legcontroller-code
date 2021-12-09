#ifndef MOTOR_SENSE_H
#define MOTOR_SENSE_H

#include <stdint.h>
#include <stdbool.h>

#define MOTOR_CURRENT_CHANNELS 3

/**
 * The most recent current value read for each motor.
 */
extern volatile uint8_t motor_current[MOTOR_CURRENT_CHANNELS];

/**
 * A flag to signal when all the current values have been updated
 * after calling 'update_motor_sense()'.
 */
extern volatile bool motor_sense_updated;

/**
 * Initialises ADC.
 */
void init_motor_sense();

/**
 * Reads the motor currents and updates the relevant variables.
 * The currents are read asynchronously so the variables won't
 * be ready when this function returns. Once all the variables
 * have been updated, the 'motor_sense_updated' flag will be set.
 */
void update_motor_sense();

#endif /* MOTOR_SENSE_H */
