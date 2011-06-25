
#ifndef PID_H_
#define PID_H_
#include <stdint.h>


// this is the struct to save the pid calculation data in.
typedef struct {
        signed short p, i, d;
        unsigned short i_limit; //total limit on the effect on I, set to 0 to disable.
        signed long error;
        signed short prev_val;
}PID_DATA;


//int16_t calculate_pid(int16_t incoming_val, int16_t goal_val, PID_DATA*pid);
int32_t calculate_pid( int16_t incoming_val,  int16_t goal_val, PID_DATA*pid);

#endif /* PID_H_ */

