
#include "radioTask.h"
#include "coos.h"

#include "taskHandles.h"
#include "lpc111x.h"
#include "rc_pwm.h"
#include "stdio.h"


// Connected to 
// 0: CT16B0_MAT0 PIO0_8
// 1: CT16B0_MAT1 PIO0_9
// 2: CT16B0_MAT2 PIO0_10
// 3: CT16B1_MAT1 PIO1_10

// PPM in = C32B0_CAP0  PIO1_5
#define RADIO_ARM_STATE (inputPPM(THROTTLE_CH) > THROTTLE_ARM_VAL && \
        inputPPM(YAW_CH) > YAW_ARM_VAL &&      \
        inputPPM(ROLL_CH) < ROLL_ARM_VAL &&     \
        inputPPM(PITCH_CH) > PITCH_ARM_VAL)

void radioTask(void *pvParameters)
{
    TASKHANDLES *taskHandles = (TASKHANDLES*)pvParameters;
    CoTickDelay(OS_MS(10));

    rcinit();
    CoTickDelay(OS_MS(500));
    int i=0;
    while(1)
    {  
        if( validPPM() )
        {   
            if(RADIO_ARM_STATE)
            {
                taskHandles->flight_control.armed = !taskHandles->flight_control.armed;
                /* Wait 1 second so the user can release the stick combo */
                if (taskHandles->flight_control.armed == 0)
                    puts("DIS");
                puts("ARMING\n");
                while(RADIO_ARM_STATE)
                    CoTickDelay(OS_MS(1000));
            }

            taskHandles->flight_control.tx_throttle = inputPPM(THROTTLE_CH); 
            taskHandles->flight_control.tx_yaw = inputPPM(YAW_CH);
            taskHandles->flight_control.tx_pitch = inputPPM(PITCH_CH);
            taskHandles->flight_control.tx_roll = inputPPM(ROLL_CH);


        }
        else
        {
            /* TODO Do I really want to do this.  
               what about tiny interrupts in signal
               Should that be handled in the interrupt call */
            /* Disarm if PPM is not valid */

            /* Need to add a safety feature here When the ppm is lost
               the flight angles requested should be set to 0,0,0
               and the throttle requested by the tx should be ratcheted down 
               over the next ~30 seconds this should safely land the quad */
            taskHandles->flight_control.armed = 0;
            taskHandles->flight_control.tx_throttle = 1000;
        }

        /* If time since last sync is greater than 500ms
           then set validPPM() to 0 */
        if (((CoGetOSTime() - last_sync) > OS_MS(500)) && taskHandles->flight_control.armed)
            radio_status = 0;



        CoTickDelay(OS_MS(20)); // NEEDS To Be faster???
    }
}
