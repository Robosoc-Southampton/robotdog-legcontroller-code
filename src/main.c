#include "motors.h"
#include "timer.h"
#include "i2c.h"
#include "leds.h"

#define UPDATE_MILLIS 10

static uint8_t led_prescaler;

int main()
{
    init_timer(UPDATE_MILLIS);
    init_motors();
    init_i2c();
    init_leds();
    
    while (1)
    {
        // Wait for timeout before executing next loop:
        while (!timer_timeout_flag);
        timer_timeout_flag = false;
        
        // Update the motors:
        update_motors(UPDATE_MILLIS);
        
        // Toggle the RUN_LED every 500 ms:
        if (led_prescaler++ == 49)
        {
            led_prescaler = 0;
            LED_RUN_TOGGLE();
        }
        
        // If the timer has already timed out before the next loop
        // it means the CPU can't keep up. Light up the ERROR LED.
        if (timer_timeout_flag)
            LED_ERR_ON();
        else
            LED_ERR_OFF();
    }
}
