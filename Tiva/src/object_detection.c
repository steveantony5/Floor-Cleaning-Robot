/*
 * object_detection.c
 *
 *  Created on: Apr 15, 2019
 *      Author: Steve Antony
 */

/**********************************************
 *        Includes
 **********************************************/

#include "object_detection.h"

uint32_t start, end;
uint32_t FLAG_UL, conv_complete = 0;
float time_pulse = 0;
float distance_send;
static char buffer_log[BUFFER];

void init_ultrasonic_sensor()
{
    SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER2);
    TimerConfigure(TIMER2_BASE, TIMER_CFG_PERIODIC_UP);


    //echo pin
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
    GPIOPinTypeGPIOInput(GPIO_PORTF_BASE, GPIO_PIN_3);
    //GPIOPadConfigSet(GPIO_PORTF_BASE,GPIO_PIN_3,GPIO_STRENGTH_2MA,GPIO_PIN_TYPE_STD_WPU);

    GPIOIntEnable(GPIO_PORTF_BASE, GPIO_INT_PIN_3);

    GPIOIntTypeSet(GPIO_PORTF_BASE,GPIO_PIN_3,GPIO_BOTH_EDGES );

    GPIOIntRegister(GPIO_PORTF_BASE,PortFIntHandler);

    GPIOIntClear(GPIO_PORTF_BASE, GPIO_INT_PIN_3);



    //trigger pin
    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1);
    GPIOPadConfigSet(GPIO_PORTF_BASE,GPIO_PIN_1,GPIO_STRENGTH_2MA,GPIO_PIN_TYPE_STD_WPU);

    UARTprintf("configured ultrasonic\n");

}


void find_object()
{
    GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_1, 0);
    vTaskDelay(pdMS_TO_TICKS( 1 ));
    GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_1, GPIO_PIN_1);
    vTaskDelay(pdMS_TO_TICKS( 10 ));
    GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_1, 0);
}


void PortFIntHandler()
{


    taskENTER_CRITICAL();
    GPIOIntClear(GPIO_PORTF_BASE, GPIO_INT_PIN_3);




        if(GPIOPinRead(GPIO_PORTF_BASE, GPIO_INT_PIN_3) == GPIO_INT_PIN_3)
        {
            HWREG(TIMER2_BASE + TIMER_O_TAV) = 0;
            TimerEnable(TIMER2_BASE,TIMER_A);
            start = TimerValueGet(TIMER2_BASE,TIMER_A);
            conv_complete = 0;

        }

        else
        {

            end = TimerValueGet(TIMER2_BASE,TIMER_A);
            TimerDisable(TIMER2_BASE,TIMER_A);
            time_pulse = end - start;
            conv_complete = 1;

        }

    taskEXIT_CRITICAL();

}

void UtrasonicTask(void *pvParameters)
{
    UARTprintf("Created ultrasonic thread\n");
    long x_ultra_id = 1003;
    xTimerHandle xTimer_ult;
    xTimer_ult = xTimerCreate("Timer_ultrasonic",               // Just a text name, not used by the kernel.
                                pdMS_TO_TICKS( 500 ),     // 1000ms
                                pdTRUE,                    // The timers will auto-reload themselves when they expire.
                                ( void * ) x_ultra_id,      // Assign each timer a unique id equal to its array index.
                                vTimerCallback_Ultra_handler// Each timer calls the same callback when it expires.
                               );



    if( (xTimer_ult == NULL ) )
    {
        // The timer was not created.
        UARTprintf("Error on timer creation - xTimer_Temp\n");
    }

    else
    {

        /*start the timer*/
         xTimerStart( xTimer_ult, 0 );


         //setup i2c
         init_ultrasonic_sensor();



         for(;;)
         {
             if(FLAG_UL == pdTRUE)
             {
                 find_object();

                 if((conv_complete == 1))
                 {
                     distance_send = (((float)(1.0/(output_clock_rate_hz/1000000))*time_pulse)/58);
//                     char D[50];
//                     memset(D,'\0',50);
//                     sprintf(D,"D %f\n",distance_send);
//                     UARTprintf("%s",D);

                     if(CN_ACTIVE)
                     {
                         xQueueSendToBack( myQueue_ultra,( void * ) &distance_send, QUEUE_TIMEOUT_TICKS ) ;
                         memset(buffer_log,'\0',BUFFER);
                         sprintf(buffer_log,"D %f\n",distance_send);
                         LOG_INFO(buffer_log)
                     }
                     else
                     {
                         if(distance_send < 30)
                         {
                             UARTprintf("Object detected\n");

                             //object detected
                             backward();

                             vTaskDelay(1000/portTICK_PERIOD_MS);

                             //normal run of motors
                            right();

                            vTaskDelay(1000/portTICK_PERIOD_MS);

                            forward();
                         }

                     }

                 }

                 FLAG_UL = pdFALSE;

             }


         }

    }
}

/**********************************************
 *         Temp timer handler
 **********************************************/
void vTimerCallback_Ultra_handler( TimerHandle_t  *pxTimer )
{
    FLAG_UL = pdTRUE;
}

