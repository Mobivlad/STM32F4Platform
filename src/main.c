/**
  ******************************************************************************
  * @file    main.c
  * @author  Ac6
  * @version V1.0
  * @date    24 June 2020
  * @brief   Default main function.
  ******************************************************************************
*/
#include "ul_heart_beat.h"

#define BLINK_FREQUENCY_5_HZ 100

int main(void)
{
    ulHeartBeatStruct heartBeat;
    heartBeat.frequency = BLINK_FREQUENCY_5_HZ;
    ulHeartBeatInit(&heartBeat);

	for(;;){
	    ulHeartBeatRun(&heartBeat);
	}
}
