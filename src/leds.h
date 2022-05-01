#ifndef LEDS_H
#define LEDS_H

#include <avr/io.h>

/**
 * Macros for turning the LEDs on and off.
 */
#define LED_RUN_ON()     (PORTB |=  (1 << PORTB0))
#define LED_RUN_OFF()    (PORTB &= ~(1 << PORTB0))
#define LED_RUN_TOGGLE() ( PINB |=  (1 <<  PINB0))

#define LED_ERR_ON()     (PORTB |=  (1 << PORTB1))
#define LED_ERR_OFF()    (PORTB &= ~(1 << PORTB1))
#define LED_ERR_TOGGLE() ( PINB |=  (1 <<  PINB1))

/**
 * Initialise the LEDs.
 */
void init_leds();

#endif /* LEDS_H */
