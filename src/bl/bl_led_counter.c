/*
 * bl_led_counter.c
 *
 *  Created on: 4 черв. 2020 р.
 *      Author: vladyslav.daliavskyi
 */

#include "bl_led_counter.h"

static drvSegmentData cathodeA = { drvSegmentPin0, drvSegmentPortD };
static drvSegmentData cathodeB = { drvSegmentPin1, drvSegmentPortD };
static drvSegmentData cathodeC = { drvSegmentPin2, drvSegmentPortD };
static drvSegmentData cathodeD = { drvSegmentPin3, drvSegmentPortD };
static drvSegmentData cathodeE = { drvSegmentPin4, drvSegmentPortD };
static drvSegmentData cathodeF = { drvSegmentPin5, drvSegmentPortD };
static drvSegmentData cathodeG = { drvSegmentPin6, drvSegmentPortD };
static drvSegmentData cathodePD = { drvSegmentPin7, drvSegmentPortD };

static drvSegmentData anode1 = { drvSegmentPin10, drvSegmentPortC };
static drvSegmentData anode2 = { drvSegmentPin11, drvSegmentPortC };
static drvSegmentData anode3 = { drvSegmentPin12, drvSegmentPortC };

static uint8_t numPosition;
static uint32_t startTime;

static blLedCounterInitLeds(){
    drvSegmentData* cathodes = {
        &cathodeA,
        &cathodeB,
        &cathodeC,
        &cathodeD,
        &cathodeE,
        &cathodeF,
        &cathodeG,
        &cathodePD
    };
    drvSegmentData* anodes = {
        &anode1,
        &anode2,
        &anode3
    };
    drvDisplayData initData = {cathodes,anodes,3};
    drvSevenSegmentInit(&initData);
    numPosition=0;
    startTime = 0;
}

/**
 * Part of blLedCounterInit() responsible for timer initialization
 */
static void blLedCounterInitTimer(){
    drvTimerInit(drvTimer0,1000);
}

static drvInitStructButton buttonInitStruct = {
    drvButtonPin0,
    drvButtonPortA,
    drvButtonHardPull,
    drvButtonPuPdNo
};

/**
 * Initial function for led counter
 */
void blLedCounterInit(){
    drvSysClockInit(1);
    blLedCounterInitLeds();
    dvrButtonInit(&buttonInitStruct);
    blLedCounterInitTimer();
}

static uint8_t started=0;

/**
 * Part of blLedCounterRun() responsible for button click detection
 */
static void buttonRun(){
    drvButtonPressType res = drvButtonIsClick(buttonInitStruct.drvButtonPort,buttonInitStruct.drvButtonPins);
        if (res==drvButtonSimplePress) {
            if(!started){
                drvTimerStart(drvTimer0);
            } else {
                drvTimerPause(drvTimer0);
            }
            started=!started;
        }
        if (res==drvButtonLongPress) {
            if(!started){
                drvTimerStop(drvTimer0);
            }
        }
}

static drvSevenSegmentSymbols symbols[] = {zero,zero,zero};

/**
 * Configure symbols array for number displaying
 * @param number display number
 */
static void setNumber(uint8_t number){
    for(uint8_t i=0;i<3;i++){
        uint8_t last_sym = number%10;
        switch (last_sym) {
            case 0:
                symbols[2-i] = zero;
                break;
            case 1:
                symbols[2-i] = one;
                break;
            case 2:
                symbols[2-i] = two;
                break;
            case 3:
                symbols[2-i] = three;
                break;
            case 4:
                symbols[2-i] = four;
                break;
            case 5:
                symbols[2-i] = five;
                break;
            case 6:
                symbols[2-i] = six;
                break;
            case 7:
                symbols[2-i] = seven;
                break;
            case 8:
                symbols[2-i] = eight;
                break;
            case 9:
                symbols[2-i] = nine;
                break;
        }
        number/=10;
    }
}

/**
 * Part of blLedCounterRun() responsible for number displaying
 */
static void ledRun(){
    if (drvIsTimePased(startTime, 7)) {
        drvSevenSegmentPrintSymbol(symbols[numPosition], numPosition);
        numPosition++;
        numPosition %= 3;
        startTime = drvSysClockGetTick();
    }
}


void blLedCounterRun(){
    setNumber(drvTimerGetTime(drvTimer0));
    buttonRun();
    ledRun();
}
