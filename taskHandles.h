
#ifndef TASKHANDLES_H
#define TASKHANDLES_H

#include <stdint.h>
#include "pid.h"

#define THROTTLE_CH 2
#define YAW_CH 3
#define ROLL_CH 0
#define PITCH_CH 1

#define MOTOR_IDLE 1000

#define A_MOTOR 0
#define B_MOTOR 1
#define C_MOTOR 2
#define D_MOTOR 3


#define X_MODE 0
#define PLUS_MODE 1
typedef struct
{

//        struct {
//                //xTaskHandle userInterface;
//                xTaskHandle radioTask;
//                xTaskHandle ledTask;
//                xTaskHandle cnt;
//                xTaskHandle stack_checker;
//        }task;
//
//        struct {
//                //xSemaphoreHandle SPI;  // spi lock
//        }lock;
//
        struct {
                PID_DATA * pid_roll;
                PID_DATA * pid_pitch;
                PID_DATA * pid_yaw;
                uint8_t flying_mode; 
//                //uint8_t led_mode;
                uint16_t pitch_roll_tx_scale;
                uint16_t yaw_tx_scale;
        }flight_settings;

        struct {
                OS_MutexID i2c;
        }lock;
            
        struct {
                uint8_t armed;
                uint8_t error;
                //unsigned char telem_mode;
                int tx_throttle, tx_yaw, tx_pitch, tx_roll;
                //unsigned int command_used_number;
                //unsigned char please_update_sensors;
        }flight_control;



}TASKHANDLES;

#endif
 


