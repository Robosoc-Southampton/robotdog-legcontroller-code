#include "rotary-encoders.h"

#include <stdbool.h>
#include <stdint.h>

#include <avr/interrupt.h>
#include <avr/io.h>

/**
 * Macros for reading the pin states.
 */
#define STATE_0A() ((PINA >> PINA7) & 1)
#define STATE_0B() ((PINA >> PINA6) & 1)
#define STATE_1A() ((PIND >> PIND1) & 1)
#define STATE_1B() ((PIND >> PIND0) & 1)
#define STATE_2A() ((PINC >> PINC7) & 1)
#define STATE_2B() ((PINC >> PINC6) & 1)

volatile int16_t rotary_count[3];

void init_rotary_encoders()
{
    // Set pins as inputs:
    DDRA &= ~(1 << DDA7) & ~(1 << DDA6);
    DDRC &= ~(1 << DDC7) & ~(1 << DDC6);
    DDRD &= ~(1 << DDD1) & ~(1 << DDD0);
    
    // Pin change masks:
    PCMSK0 = (1 <<  PCINT7) | (1 <<  PCINT6);  // Port A
    PCMSK2 = (1 << PCINT23) | (1 << PCINT22);  // Port C
    PCMSK3 = (1 << PCINT25) | (1 << PCINT24);  // Port D
    
    // Enable pin change interrupts on port A, C and D
    PCICR |= (1 << PCIE3) | (1 << PCIE2) | (1 << PCIE0);
    
    sei();
}

ISR(PCINT0_vect)
{
    static bool last_state_a = false;
    static bool last_state_b = false;
    
    bool curr_state_a = STATE_0A();
    bool curr_state_b = STATE_0B();
    
    // If neither pin has changed, ignore the interrupt.
    // This can happen when there is a very short pulse from switch bouncing.
    if (curr_state_a == last_state_a)
        if (curr_state_b == last_state_b)
            return;
    
    // Calculate direction:
    if (curr_state_a != last_state_b)
        rotary_count[0]++;
    else
        rotary_count[0]--;
    
    last_state_a = curr_state_a;
    last_state_b = curr_state_b;
}

ISR(PCINT3_vect)
{
    static bool last_state_a = false;
    static bool last_state_b = false;
    
    bool curr_state_a = STATE_1A();
    bool curr_state_b = STATE_1B();
    
    // If neither pin has changed, ignore the interrupt.
    // This can happen when there is a very short pulse from switch bouncing.
    if (curr_state_a == last_state_a)
        if (curr_state_b == last_state_b)
            return;
    
    // Calculate direction:
    if (curr_state_a != last_state_b)
        rotary_count[1]++;
    else
        rotary_count[1]--;
    
    last_state_a = curr_state_a;
    last_state_b = curr_state_b;
}

ISR(PCINT2_vect)
{
    static bool last_state_a = false;
    static bool last_state_b = false;
    
    bool curr_state_a = STATE_2A();
    bool curr_state_b = STATE_2B();
    
    // If neither pin has changed, ignore the interrupt.
    // This can happen when there is a very short pulse from switch bouncing.
    if (curr_state_a == last_state_a)
        if (curr_state_b == last_state_b)
            return;
    
    // Calculate direction:
    if (curr_state_a != last_state_b)
        rotary_count[2]++;
    else
        rotary_count[2]--;
    
    last_state_a = curr_state_a;
    last_state_b = curr_state_b;
}
