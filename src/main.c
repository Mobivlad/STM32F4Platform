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

#define BLINK_FREQUENCY_5_HZ 200

 ulHeartBeatStruct heartBeat;
int main(void)
{
    HAL_Init();

    heartBeat.frequency = BLINK_FREQUENCY_5_HZ;
    ulHeartBeatInit(&heartBeat);

    xTaskCreate(ulHeartBeatTaskFunction, (signed char *) "HEART_BEAT", configMINIMAL_STACK_SIZE,
                (void*) &heartBeat, 6, (xTaskHandle *) NULL);

    vTaskStartScheduler();
}
