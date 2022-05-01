#include "timer.h"

#include <avr/interrupt.h>
#include <stdint.h>
#include <stdbool.h>

static uint8_t timer_millis;

uint8_t timer_timeout_millis;
volatile bool timer_timeout_flag;

void init_timer(uint8_t timeout_millis)
{
    timer_millis = 0;
    timer_timeout_millis = timeout_millis;
    
    // Set up timer:
    cli();
    TCCR0A = (1 << WGM01);              // CTC mode
    TCCR0B = (1 << CS01) | (1 << CS00); // 1/64th prescaler
    TCNT0 = 0;                          // Reset counter
    OCR0A = F_CPU / 64 / 1000 - 1;      // Set TOP for 1000 Hz overflows
    TIMSK0 = (1 << OCIE0A);             // Enable interrupt
    sei();
}

ISR(TIMER0_COMPA_vect )
{
    static uint8_t next_timeout_millis = 0;
    
    timer_millis++;
    
    if (timer_millis == next_timeout_millis)
    {
        next_timeout_millis += timer_timeout_millis;
        timer_timeout_flag = true;
    }
}
