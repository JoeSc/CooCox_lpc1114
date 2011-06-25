#include "coos.h"
#include "lpc111x.h"

#include "taskHandles.h"
#include "flightTask.h"
#include "bma180.h"
#include "itg3200.h"
#include "hmc5843.h"
#include "fixed_DCM.h"
#include "pid.h"
#include "rc_pwm.h"


#include "stdio.h"


void updateSensors(uint64_t last_calc)
{
    update_bma180();
    update_itg3200();
    update_hmc5843();
    calculate_heading_hmc5843(pitch, roll);

    G_Dt = ((CoGetOSTime()-last_calc)*(65000/CFG_SYSTICK_FREQ));
    Matrix_update( itg3200_gyro_x, itg3200_gyro_y, itg3200_gyro_z-90, 
            bma180_acc_x, bma180_acc_y, bma180_acc_z);
    Normalize();
    Drift_correction(fix16_cos(-hmc5843_heading), fix16_sin(-hmc5843_heading));
    Euler_angles();
}


void initSensors()
{
    int error=0;
    if ( (error = init_bma180(0x02, 0x02) << 2 |
            init_itg3200() << 1 |
            init_hmc5843()) )
    {
        printf("I2C ERROR 0x%x",error);
        while(1);
        /* Since the flightTask should be highest priority 
         * this should kill the board I will probably have 
         * to add a setup of the esc's so that they are truly off
         */
    }
    puts("I2C INITED\n");
}

void zeroMotors()
{
    setRC(A_MOTOR, MOTOR_IDLE);
    setRC(B_MOTOR, MOTOR_IDLE);
    setRC(C_MOTOR, MOTOR_IDLE);
    setRC(D_MOTOR, MOTOR_IDLE);
}

#define FLIGHT_RATE 20 // in milliseconds
void flightTask( void *param)
{
    TASKHANDLES *taskHandles = (TASKHANDLES*)param;
    zeroMotors();

    /* -----------------    P    I   D   limit   */
    PID_DATA pid_pitch = { 700, 001, 000, 100 };
    PID_DATA pid_roll =  { 700, 001, 000, 100 };
    PID_DATA pid_yaw =   {1200, 005, 000, 100 };
    taskHandles->flight_settings.pid_pitch = &pid_pitch;
    taskHandles->flight_settings.pid_roll = &pid_roll;
    taskHandles->flight_settings.pid_yaw = &pid_yaw;

    taskHandles->flight_control.tx_throttle = 1000;
    taskHandles->flight_control.tx_yaw = 1500;
    taskHandles->flight_control.tx_pitch = 1500;
    taskHandles->flight_control.tx_roll = 1500;
    taskHandles->flight_settings.pitch_roll_tx_scale = 16;
    taskHandles->flight_settings.yaw_tx_scale = 16;

    taskHandles->flight_settings.flying_mode = X_MODE;

    initSensors();



    uint64_t last_calc = CoGetOSTime();
    int i =0;
    while(1)
    {
        CoTickDelay(OS_MS(FLIGHT_RATE));

        updateSensors(last_calc);
        last_calc = CoGetOSTime();

        int16_t pitch_offset = 0,roll_offset = 0,yaw_offset = 0;

        pitch_offset = calculate_pid( pitch, taskHandles->flight_control.tx_pitch, &pid_pitch);
        roll_offset = calculate_pid( 1500, taskHandles->flight_control.tx_roll, &pid_roll);
        yaw_offset = calculate_pid( 1500, taskHandles->flight_control.tx_yaw, &pid_yaw);

        if (taskHandles->flight_control.armed == 1)
        {
            if(taskHandles->flight_settings.flying_mode == X_MODE)
            {
                setRC(A_MOTOR, taskHandles->flight_control.tx_throttle + pitch_offset + roll_offset + yaw_offset);
                setRC(B_MOTOR, taskHandles->flight_control.tx_throttle - pitch_offset - roll_offset + yaw_offset);
                setRC(C_MOTOR, taskHandles->flight_control.tx_throttle - pitch_offset + roll_offset - yaw_offset);
                setRC(D_MOTOR, taskHandles->flight_control.tx_throttle + pitch_offset - roll_offset - yaw_offset);
            }
        }
        else
            zeroMotors();



    }
}

