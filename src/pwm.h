#ifndef PWM_H
#define PWM_H

#include <stdint.h>
#include <stdbool.h>

#define PWM_CHANNELS 3

/**
 * Initialises the logic.
 */
void init_pwm();

/**
 * Enables/disables a PWM channel. While a channel is disabled, no pulses
 * are sent on the pin.
 */
void pwm_set_enabled(uint8_t channel, bool enabled);

/**
 * Returns whether a channel is enabled.
 */
bool pwm_get_enabled(uint8_t channel);

/**
 * Sets the PWM pulse width of a channel, where 0 corresponds to 1 ms
 * and UINT16_MAX corresponds to 2 ms.
 */
void pwm_set_value(uint8_t channel, uint16_t value);

/**
 * Gets the PWM pulse width of a channel, where 0 corresponds to 1 ms
 * and UINT16_MAX corresponds to 2 ms.
 */
uint16_t pwm_get_value(uint8_t channel);

#endif /* PWM_H */
