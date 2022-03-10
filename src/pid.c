void update_pid(struct motor_pid *mp, uint16_t new_pos)
{
    int16_t error = new_pos - mp->position;

    if (mp->integral > -integral_limit && mp->integral < integral_limit)
        mp->integral += error;

    int16_t derivative = error - mp->last_error;

    int16_t correct = kp * error + ki * integral + kd * derivative;

    int16_t tpwm = mp->pwm + correct;
    if (tpwm < lower_limit)
        mp->pwm = lower_limit;
    else if (tpwm > upper_limit)
        mp->pwm = upper_limit;
    else
        mp->pwm = tpwm;
}

/*
struct motor_pid mp1;
while (1)
{
    uint16_t rot = read_rotary()
        update_pid(&mp1, rot);
    set_pwm(mp1.pwm);
}
*/