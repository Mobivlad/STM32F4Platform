/*
 * main.c
 *
 *  Created on: 1 ����. 2020 �.
 *      Author: vladyslav.daliavskyi
 */

//include file of timer blink BL
#include "bl_timer_blink.h"

//include file of blink on button toggle BL
#include "bl_button_blink.h"

int main(){
    //initial functions
    blButtonBlinkInit();
    blBlinkInit(500);

    while(1){
        blBlinkRun();
        blButtonBlinkRun();
    }
}
