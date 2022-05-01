#include "motors.h"

#include "pid.h"
#include "pwm.h"
#include "rotary-encoders.h"

#include <stdint.h>
#include <avr/io.h>
#include <util/atomic.h>

#define PWM_MOTOR_PORTS 3

static struct motor_pid pids[PWM_MOTOR_PORTS] = {
    { .kp = 1, .ki = 1, .kd = 1, .integral_limit = 1, .lower_limit = 1, .upper_limit = 1 },
    { .kp = 1, .ki = 1, .kd = 1, .integral_limit = 1, .lower_limit = 1, .upper_limit = 1 },
    { .kp = 1, .ki = 1, .kd = 1, .integral_limit = 1, .lower_limit = 1, .upper_limit = 1 }
};

struct motor motors[MOTOR_PORTS] = {
    { .type = BRUSHLESS, .precision = 4000 },
    { .type = BRUSHLESS, .precision = 4000 },
    { .type = BRUSHLESS, .precision = 4000 }
};

uint8_t motor_calibrate_flags = 0;
uint8_t motor_enabled_flags = 0;

void init_motors()
{
    init_rotary_encoders();
    init_pwm();
    
    // ***** Set up limit switches *****
    // Set as inputs:
    DDRA &= ~(1 << DDA5);
    DDRD &= ~(1 << DDD3) & ~(1 << DDD2);
    // Enable pullups:
    PORTA |= (1 << PORTA5);
    PORTD |= (1 << PORTD3) | (1 << PORTD2);
    
    // ***** Set up cuttoff MOSFET pins *****
    // Set as outputs:
    DDRC |= (1 << DDC4) | (1 << DDC3) | (1 << DDC2);
    // Set LOW (disable motors - only applies to servos):
    PORTA &= ~(1 << PORTA4) & ~(1 << PORTA3) & ~(1 << PORTA2);
}

void update_motors(uint8_t millis)
{
    for (uint8_t port = 0; port < MOTOR_PORTS; port++)
    {
        struct motor *motor = &motors[port];
        struct motor_pid *pid = &pids[port];
        
        switch (motor->type)
        {
            case BRUSHLESS:
                // Only first 3 ports can be brushless:
                if (port >= PWM_MOTOR_PORTS)
                    break;
                
                ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
                {
                    // Update the PID's target position:
                    pid->position = motor->target_pos;
                    
                    // Update the motor's actual position
                    // from rotary encoders:
                    motor->actual_pos = rotary_count[port];
                }
                
                // If calibrate bit is set, calibrate rotary encoder
                // and clear calibrate bit:
                if (motor_calibrate_flags & (1 << port))
                {
                    SET_ROTARY_COUNT(port, 0);
                    motor_calibrate_flags &= ~(1 << port);
                }
                
                // If enable flag is set/cleared, enable/disable PWM:
                if (motor_enabled_flags & (1 << port))
                {
                    pwm_set_enabled(port,true);
                }
                else
                {
                    pwm_set_enabled(port, false);
                    break;
                }
                
                // Update PID:
                update_pid(pid, motor->actual_pos);
                
                // Update PWM:
                pwm_set_value_raw(port, pid->pwm);
                
                break;
            default:
                pwm_set_enabled(port, false);
                break;
        }
    }
}
