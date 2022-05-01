#include "leds.h"

#include <avr/io.h>

void init_leds()
{
    // Set LED pins as outputs:
    DDRB |= (1 << DDB1) | (1 << DDB0);
    
    // Turn LEDs off:
    PORTB &= ~(1 << PORTB1) & (1 << PORTB0);
}
