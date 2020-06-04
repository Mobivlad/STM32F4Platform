/*
 * bl_number_display.c
 *
 *  Created on: 3 черв. 2020 р.
 *      Author: vladyslav.daliavskyi
 */

#include "bl_number_display.h"

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

void blDisplayNumberInit(){
    drvSysClockInit(1);
    drvSegmentData* cathodes = {
        &cathodeA,
        &cathodeB,
        &cathodeC,
        &cathodeD,
        &cathodeE,
        &cathodeF,
        &cathodeG,
        &cathodePD,
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

static drvSevenSegmentSymbols symbols[] = {one,two,three};

void blDisplaySetNumber(uint8_t number){
    for(uint8_t i=0;i<3;i++){
        uint8_t last_sym = number%10;
        switch (last_sym) {
            case 0:
                symbols[i] = zero;
                break;
            case 1:
                symbols[i] = one;
                break;
            case 2:
                symbols[i] = two;
                break;
            case 3:
                symbols[i] = three;
                break;
            case 4:
                symbols[i] = four;
                break;
            case 5:
                symbols[i] = five;
                break;
            case 6:
                symbols[i] = six;
                break;
            case 7:
                symbols[i] = seven;
                break;
            case 8:
                symbols[i] = eight;
                break;
            case 9:
                symbols[i] = nine;
                break;
        }
        number/=10;
    }
}

void blDisplayNumberRun(){
    if(drvIsTimePased(startTime,7)){
        switch(numPosition){
            case 0:
                drvSevenSegmentPrintSymbol(one,0);
                break;
            case 1:
                drvSevenSegmentPrintSymbol(two,1);
                break;
            case 2:
                drvSevenSegmentPrintSymbol(three, 2);
                break;
        }
        numPosition++;
        numPosition%=3;
        startTime = drvSysClockGetTick();
    }
}

