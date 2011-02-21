#include "coos.h"


#include "ledTask.h"
#include "taskHandles.h"
#include "lpc111x.h"
#include "stdio.h"




void ledTask(void *pvParameters)
{
    TASKHANDLES *taskHandles = (TASKHANDLES*)pvParameters;
    SYSTICK_LED_IOCON = 0x0;
    SYSTICK_LED_DIR = (1<<SYSTICK_LED_PIN);
  
    CoTickDelay(100);

   
    unsigned int i;
    unsigned int delay = 500;
    for ( i=0; ;i++)
    {
      if ( taskHandles->flight_control.armed )
            delay = 100;
        else
            delay = 500;

        SYSTICK_LED_DATA ^= (1<<SYSTICK_LED_PIN);
        CoTickDelay(OS_MS(delay));
    }
}


