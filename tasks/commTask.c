#include "coos.h"
#include "lpc111x.h"

#include "taskHandles.h"
#include "stdio.h"

void commTask( void *param)
{
    TASKHANDLES *taskHandles = (TASKHANDLES*)param;
    CoTickDelay(OS_MS(10));
    
    while (1)
    {
        CoTickDelay(OS_MS(10));
    


    }
}

