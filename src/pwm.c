#include "pwm.h"

#include <stdint.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/atomic.h>
#include <stdbool.h>

#define TIMER_PRESC (8)
#define TIMER_TOP (F_CPU / TIMER_PRESC / 50 / PWM_CHANNELS - 1)

#define TIMER_ONE_MS (F_CPU / TIMER_PRESC / 1000)

static volatile uint8_t *const port[PWM_CHANNELS] = { &PORTB, &PORTD, &PORTD };
static const uint8_t pin_mask[PWM_CHANNELS] = { (1 << PORTB4), (1 << PORTD6), (1 << PORTD7) };
static uint8_t pin_mask_enable[PWM_CHANNELS] = { 0 };

static uint16_t pwm_value[PWM_CHANNELS];
static uint8_t channel = PWM_CHANNELS - 1;

void init_pwm()
{
    // Set pins as outputs:
    DDRB |= (1 << DDB4);
    DDRD |= (1 << DDD7) | (1 << DDD6);
    
    // Set default value for all channels:
    for (uint8_t i = 0; i < PWM_CHANNELS; i++)
        pwm_set_value(i, 0);
    
    // Set up timer:
    OCR1A = TIMER_TOP;
    TCCR1A = (1 << WGM11) | (1 << WGM10);  // Fast PWM mode, TOP = OCR1A
    TCCR1B = (1 << WGM13) | (1 << WGM12)   // "
           | (1 << CS11);                  // 8th prescaler
    TCCR1C = 0;
    TIMSK1 = (1 << OCIE1B) | (1 << TOIE1); // Enable COMPB and OVF interrupts
    
    sei();
}

void pwm_set_enabled(uint8_t channel, bool enabled)
{
    if (enabled)
        pin_mask_enable[channel] = pin_mask[channel];
    else
        pin_mask_enable[channel] = 0;
}

bool pwm_get_enabled(uint8_t channel)
{
    return (bool) pin_mask_enable[channel];
}

void pwm_set_value(uint8_t channel, int16_t value)
{
    uint16_t value_raw = (TIMER_ONE_MS * 3/2) + ((TIMER_ONE_MS * value) >> 16) - 1;
    
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
    {
        pwm_value[channel] = value_raw;
    }
}

int16_t pwm_get_value(uint8_t channel)
{
    // Reverse of pwm_set_value but rearranged to avoid slow runtime division:
    return (pwm_value[channel] - (TIMER_ONE_MS * 3/2) + 1) * (((uint32_t) 1 << 24) / TIMER_ONE_MS) / (1 << 8);
}

void pwm_set_value_raw(uint8_t channel, uint16_t value)
{
    // Constrain value between 0.5 and 2.5 ms:
    if (value <= TIMER_ONE_MS * 1/2)
        value = TIMER_ONE_MS * 1/2;
    else if (value >= TIMER_ONE_MS * 5/2)
        value = TIMER_ONE_MS * 5/2;
    
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
    {
        pwm_value[channel] = value;
    }
}

uint16_t pwm_get_value_raw(uint8_t channel)
{
    return pwm_value[channel];
}

ISR(TIMER1_OVF_vect)
{
    *port[channel] |= pin_mask_enable[channel];
}

ISR(TIMER1_COMPB_vect)
{
    *port[channel] &= ~pin_mask[channel];
    
    channel++;
    
    if (channel >= PWM_CHANNELS)
        channel = 0;
    
    OCR1B = pwm_value[channel];
}
