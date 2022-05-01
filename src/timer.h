#ifndef TIMER_H
#define TIMER_H

#include <stdint.h>
#include <stdbool.h>

/**
 * The number of milliseconds between timer timeouts.
 */
extern uint8_t timer_timeout_millis;

/**
 * A flag which is set each time the timer times out.
 * Must be cleared manually.
 */
extern volatile bool timer_timeout_flag;

/**
 * Initialise timer.
 */
void init_timer(uint8_t timeout_millis);

#endif /* TIME_H */
