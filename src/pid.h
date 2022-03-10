#ifndef PID_H
#define PID_H
#include <stdint.h>

struct motor_pid
{
    uint16_t position;
    int16_t last_error, integral;
    uint16_t kp, ki, kd;
    uint16_t pwm;
    uint16_t integral_limit, lower_limit, upper_limit;
};

void update_pid(struct motor_pid *mp, uint16_t new_pos);

#endif // PID_H