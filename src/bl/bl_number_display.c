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
static uint8_t startTime;

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


void blDisplayNumberRun(){
    if(drvIsTimePased(startTime,40)){
        switch(numPosition){
            case 0:
                drvSevenSegmentPrintNumber(one,0);
                break;
            case 1:
                drvSevenSegmentPrintNumber(two,1);
                break;
            case 2:
                drvSevenSegmentPrintNumber(three,2);
                break;
        }
        numPosition++;
        startTime = drvSysClockGetTick();
    }

}

