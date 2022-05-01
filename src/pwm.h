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
 * Sets the PWM pulse width of a channel, where INT16_MIN corresponds to 1 ms,
 * 0 corresponds to 1.5 ms, and INT16_MAX corresponds to 2 ms.
 */
void pwm_set_value(uint8_t channel, int16_t value);

/**
 * Gets the PWM pulse width of a channel, where INT16_MIN corresponds to 1 ms,
 * 0 corresponds to 1.5 ms, and INT16_MAX corresponds to 2 ms.
 */
int16_t pwm_get_value(uint8_t channel);

/**
 * Sets the raw timer value for a PWM channel.
 * Note that the pulse width is contrained between 0.5 and 2.5 ms.
 */
void pwm_set_value_raw(uint8_t channel, uint16_t value);

/**
 * Gets the raw timer value for a PWM channel.
 */
uint16_t pwm_get_value_raw(uint8_t channel);

#endif /* PWM_H */
