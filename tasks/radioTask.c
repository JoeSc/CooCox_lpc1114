
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

void radioTask(void *pvParameters)
{
    TASKHANDLES *taskHandles = (TASKHANDLES*)pvParameters;
    CoTickDelay(OS_MS(10));
    
    rcinit();
    CoTickDelay(OS_MS(500));

    while(1)
    {  
        if(validPPM() )
        {   
            //int i = 0;
            //for ( i =0;i<7;i++)
            //    printf("CH %d = %d\n",i,inputPPM(i));
            if(inputPPM(2) > THROTTLE_ARM_VAL &&
                   inputPPM(3) > YAW_ARM_VAL &&
                   inputPPM(0) < ROLL_ARM_VAL &&
                   inputPPM(1) > PITCH_ARM_VAL)
            {
                taskHandles->flight_control.armed = !taskHandles->flight_control.armed;
                /* Wait 1 second so the user can release the stick combo */
                if (taskHandles->flight_control.armed == 0)
                    puts("DIS");
                puts("ARMING\n");
                CoTickDelay(OS_MS(1000));
            }

        }
        else
            /* TODO Do I really want to do this.  
               what about tiny interrupts in signal
               Should that be handled in the interrupt call */
            /* Disarm if PPM is not valid */
            taskHandles->flight_control.armed = 0;


        CoTickDelay(OS_MS(500)); // NEEDS To Be faster???
    }
}
