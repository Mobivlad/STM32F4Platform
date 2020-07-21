/**
  ******************************************************************************
  * @file    main.c
  * @author  Ac6
  * @version V1.0
  * @date    01-December-2013
  * @brief   Default main function.
  ******************************************************************************
*/


#include "ul_heart_beat.h"
#include "ul_moving_average.h"

#define BLINK_FREQUENCY_5_HZ 200
#define ADC_FREQUENCY_1_HZ 1

 ulHeartBeatStruct heartBeat;
 ulMovingAvarege_struct adc;
int main(void)
{
    HAL_Init();

    heartBeat.frequency = BLINK_FREQUENCY_5_HZ;
    ulHeartBeatInit(&heartBeat);

    adc.adc = drvADC1;
    adc.frequency = ADC_FREQUENCY_1_HZ;

    ulMovingAvaregeInit(&adc);

    xTaskCreate(ulHeartBeatTaskFunction, (signed char *) "HEART_BEAT", configMINIMAL_STACK_SIZE,
                (void*) &heartBeat, 6, (xTaskHandle *) NULL);

    xTaskCreate(ulMovingAvaregeTaskFunction, (signed char *) "ADC", configMINIMAL_STACK_SIZE,
                    (void*) &adc, 6, (xTaskHandle *) NULL);

    vTaskStartScheduler();
}
