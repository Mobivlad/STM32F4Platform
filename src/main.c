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


int main(void)
{
    ulHeartBeatStruct heartBeat;
    heartBeat.frequency = 200;
    ulHeartBeatInit(&heartBeat);

	for(;;){
	    ulHeartBeatRun(&heartBeat);
	}
}
