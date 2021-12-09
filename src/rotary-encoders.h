#ifndef ROTARY_ENCODERS_H
#define ROTARY_ENCODERS_H

#include <stdint.h>
#include <util/atomic.h>

#define GET_ROTARY_COUNT(channel)        ({ int16_t count; ATOMIC_BLOCK(ATOMIC_RESTORESTATE){ count = rotary_count[channel]; } count;})
#define SET_ROTARY_COUNT(channel, count) ({ ATOMIC_BLOCK(ATOMIC_RESTORESTATE){ rotary_count[channel] = (count); } })

/**
 * The rotary encoder count of each motor.
 * NOTE: Access must be done atomically.
 *       You can use the above macros to do this.
 */
extern volatile int16_t rotary_count[3];

/**
 * Initialise logic.
 */
void init_rotary_encoders();

#endif /* ROTARY_ENCODERS_H */
