#include "coos.h"
#include "taskHandles.h"

#include <stdio.c>
#include <uart.h>
#include <cpu_init.h>
#include <i2c.h>

#include "ledTask.h"
#include "radioTask.h"

#define PRIORITY_LED_TASK     3
#define PRIORITY_RADIO_TASK   2

#define SIZE_LED_TASK         128
#define SIZE_RADIO_TASK       256

OS_STK stkLED[SIZE_LED_TASK];
OS_STK stkRADIO[SIZE_RADIO_TASK];


int main(void)
{
    cpuInit();
    uartInit(BAUDRATE);
    i2cInit();

    TASKHANDLES System;
    System.flight_control.armed=0;
    System.lock.i2c = CoCreateMutex ( );

    CoInitOS();

    CoCreateTask( ledTask,
            &System,
            PRIORITY_LED_TASK,
            &stkLED[SIZE_LED_TASK-1],
            SIZE_LED_TASK );

    CoCreateTask( radioTask,
            &System,
            PRIORITY_RADIO_TASK,
            &stkRADIO[SIZE_RADIO_TASK-1],
            SIZE_RADIO_TASK );


    printf("Starting OS!!\n");

    CoStartOS();
    printf("ERROR");
    while(1);
}


