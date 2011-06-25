
#include "pid.h"
#include "fixmath.h"

#define INTEGRAL_SCALING_FACTOR (1<<10) //(1<<10) = 1,024.

void clip(long * val, long limit)
{
    if (*val > limit) *val = limit;
    else if (*val < -limit) *val = -limit;
}

int32_t calculate_pid(int16_t incoming_val, int16_t goal_val, PID_DATA*pid)
{
    fix16_t delta = fix16_sadd(goal_val, -incoming_val);

    pid->error += delta; 

    clip(&(pid->error),(pid->i_limit) * INTEGRAL_SCALING_FACTOR/pid->i );

    int16_t d_val = (incoming_val - pid->prev_val);
    pid->prev_val = incoming_val;
    

    return ( (delta * pid->p) + (pid->error * pid->i)/100 + (d_val * pid->d));

}


