#include "coos.h"

#include "taskHandles.h"
#include "flightTask.h"
#include "bma180.h"
#include "itg3200.h"
#include "hmc5843.h"

#include "stdio.h"


void flightTask( void *param)
{
    TASKHANDLES *taskHandles = (TASKHANDLES*)param;
    CoTickDelay(OS_MS(10));


    if ( init_bma180(0x02, 0x02) |
            init_itg3200() |
            init_hmc5843() )
    {
        taskHandles->flight_control.error = 1;
        puts("I2C ERROR");
        while(1);
        /* Since the flightTask should be highest priority 
         * this should kill the board I will probably have 
         * to add a setup of the esc's so that they are truly off
         */
    }



    while(1)
    {
        //CoTickDelay(OS_MS(1000));
        int i = 100;
        printf("%d",i);
    }
}

