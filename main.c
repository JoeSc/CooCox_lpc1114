//#include "lpc11xx_syscon.h"
//#include "lpc11xx_gpio.h"
#include "coos.h"

#include <stdio.c>
#include <uart.h>
#include <cpu_init.h>


#define LED0      GPIO_Pin_1
#define LED1      GPIO_Pin_2

#define PRIORITY_BLINK_TASK0  2		/* Priority of Blink task 0 */
#define PRIORITY_BLINK_TASK1  3     /* Priority of Blink task 1 */

#define SIZE_BLINK_TASK0	  128    /* Stack size of Blink task 0 */
#define SIZE_BLINK_TASK1      128    /* Stack size of Blink task 1 */

OS_STK stkBlink0[SIZE_BLINK_TASK0];	/* Stack of Blink task 0 */
OS_STK stkBlink1[SIZE_BLINK_TASK1]; /* Stack of Blink task 0 */



void taskBlink0(void *param);
void taskBlink1(void *param);



int main(void)
{
    cpuInit();
    uartInit(BAUDRATE);

    int System = 24;
    

    CoInitOS();

	/* Create Blink Task 0 */
	CoCreateTask( taskBlink0,
				  &System,
				  PRIORITY_BLINK_TASK0,
			      &stkBlink0[SIZE_BLINK_TASK0-1],
			      SIZE_BLINK_TASK0 );

	/* Create Blink Task 1 */
	CoCreateTask( taskBlink1,
				   &System,
				  PRIORITY_BLINK_TASK1,
			      &stkBlink1[SIZE_BLINK_TASK1-1],
			      SIZE_BLINK_TASK1 );
printf("Starting OS!!\n");

    	CoStartOS();

	while(1);
}

void taskBlink0(void *param)
{
    int cnt = 0;
	for(;;) {
        printf("Blink0 = %d\n",cnt++);
		CoTickDelay(20);	/* Delay 200ms */

	}
}

void taskBlink1(void *param)
{
    int cnt = 0;

    int *val = (int*)param;
    printf("Task 1 GOT %d\n",*val);
	for(;;) {
        printf("Blink1 = %d\n",cnt++);
		CoTickDelay(40);	/* Delay 400ms */
	}
}


