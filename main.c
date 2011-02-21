#include "coos.h"
#include "taskHandles.h"

#include <stdio.c>
#include <uart.h>
#include <cpu_init.h>
#include "ledTask.h"

#define PRIORITY_LED_TASK     3

#define SIZE_LED_TASK         128

OS_STK stkLED[SIZE_LED_TASK];

int main(void)
{
    cpuInit();
    uartInit(BAUDRATE);

    TASKHANDLES System;
    System.flight_control.armed=0;


    CoInitOS();

    CoCreateTask( ledTask,
            &System,
            PRIORITY_LED_TASK,
            &stkLED[SIZE_LED_TASK-1],
            SIZE_LED_TASK );

    printf("Starting OS!!\n");

    CoStartOS();
    printf("ERROR");
    while(1);
}


