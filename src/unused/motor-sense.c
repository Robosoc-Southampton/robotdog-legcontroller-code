#include "motor-sense.h"

#include <stdint.h>
#include <stdbool.h>

#include <avr/interrupt.h>
#include <avr/io.h>

volatile uint8_t motor_current[MOTOR_CURRENT_CHANNELS];
volatile bool motor_sense_updated = false;

void init_motor_sense()
{
    // Set pins as inputs, no pullup:
    DDRA &= ~(1 << DDA2) & ~(1 << DDA1) & ~(1 << DDA0);
    PORTA &= ~(1 << PORTA2) & ~(1 << PORTA1) & ~(1 << PORTA0);
    
    // Set up ADC:
    ADMUX = (1 << ADLAR); // left adjust, channel 0
    ADCSRA = (1 <<  ADEN) // enable ADC
           | (1 <<  ADIE) // enable interrupt
           | (1 << ADPS2) | (1 << ADPS1); // 64th prescaler
    DIDR0 = (1 << ADC2D) | (1 << ADC1D) | (1 << ADC0D); // disable digital inputs
    
    sei();
}

void update_motor_sense()
{
    motor_sense_updated = false;
    ADCSRA |= (1 << ADSC); // start first conversion
}

ISR(ADC_vect)
{
    static uint8_t channel = 0;
    
    motor_current[channel] = ADCH;
    channel++;
    
    // If we've haven't reached the last channel yet continue
    // to the next conversion, else stop and reset:
    if (channel < MOTOR_CURRENT_CHANNELS)
    {
        ADMUX = (1 << ADLAR) | channel; // set ADC channel (and left adjust result)
        ADCSRA |= (1 << ADSC); // start next conversion
    }
    else
    {
        channel = 0;
        motor_sense_updated = true;
        ADMUX = (1 << ADLAR) | channel; // set ADC channel (and left adjust result)
    }
}
