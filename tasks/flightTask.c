#include "coos.h"
#include "lpc111x.h"

#include "taskHandles.h"
#include "flightTask.h"
#include "bma180.h"
#include "itg3200.h"
#include "hmc5843.h"
#include "fixed_DCM.h"

#include "stdio.h"


void flightTask( void *param)
{
    TASKHANDLES *taskHandles = (TASKHANDLES*)param;
    CoTickDelay(OS_MS(10));

    IOCON_PIO2_6 = IOCON_PIO2_6_FUNC_GPIO;
    GPIO_GPIO2DIR |= (1<<6);
    GPIO_GPIO2DATA &= ~(1<<6);

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
    puts("I2C INITED\n");


    while(1)
    {
        CoTickDelay(OS_MS(40));

        GPIO_GPIO2DATA |= (1<<6);

        update_bma180();
        update_itg3200();
        update_hmc5843();
        calculate_heading_hmc5843( -roll, pitch);
        
        G_Dt = 2621;
        Matrix_update( itg3200_gyro_x, itg3200_gyro_y, itg3200_gyro_z-90, 
                bma180_acc_x, bma180_acc_y, bma180_acc_z);
        Normalize();
//        Drift_correction(fix16_cos(-hmc5843_heading),fix16_sin(-hmc5843_heading));
        Drift_correction(fix16_sin(-hmc5843_heading), -fix16_cos(-hmc5843_heading));
        Euler_angles();
        GPIO_GPIO2DATA &= ~(1<<6);

        //        printf("%d,%d,%d,",bma180_acc_x,bma180_acc_y,bma180_acc_z);
        //        printf("%d,%d,%d\n",itg3200_gyro_x,itg3200_gyro_y,itg3200_gyro_z);
        printf("%d, %d, %d\n",roll,pitch,yaw);
//        printf("%8d\%8d\t%8d\n",hmc5843_mag_x,hmc5843_mag_y,fix16_to_int(fix16_mul(fix16_from_dbl(57.2957795131),hmc5843_heading)));

    }
}

