/*
 * main.c
 *
 *  Created on: 1 черв. 2020 р.
 *      Author: vladyslav.daliavskyi
 */

#include "bl_timer_blink.h"
#include "bl_button_toggle.h"

ledData leds[blLedCount] =
{
    {blRedLedPin, blRedLedPort, 0},
    {blGreenLedPin, blRedLedPort, 0}
};

blButtonData buttons[blButtonCount] =
{
    {blButton1Pin, blButton1Port, 0},
};

int main(){
    blInitBlink(&leds[0]);
    blButtonLedToggleInit(&buttons[0],&leds[1]);
    while(1){
        blBlink(&leds[0],500);
        blToggleIfClick(&buttons[0],&leds[1]);
    }
}
