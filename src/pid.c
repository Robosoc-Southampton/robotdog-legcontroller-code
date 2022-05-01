#include "pid.h"

void update_pid(struct motor_pid *mp, int16_t new_pos)
{
    int16_t error = new_pos - mp->position;

    if (mp->integral > -mp->integral_limit && mp->integral < mp->integral_limit)
        mp->integral += error;

    int16_t derivative = error - mp->last_error;
    mp->last_error = error;
    int16_t correct = mp->kp * error + mp->ki * mp->integral + mp->kd * derivative;
    uint16_t tpwm = mp->pwm - correct;
    
    if (tpwm < mp->lower_limit)
        mp->pwm = mp->lower_limit;
    else if (tpwm > mp->upper_limit)
        mp->pwm = mp->upper_limit;
    else
        mp->pwm = tpwm;
}
