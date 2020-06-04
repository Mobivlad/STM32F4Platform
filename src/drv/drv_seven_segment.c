/*
 * drv_seven_segment.c
 *
 *  Created on: 3 черв. 2020 р.
 *      Author: vladyslav.daliavskyi
 */

#include "drv_seven_segment.h"

/// Enumeration of pin type
typedef enum {
    drvPinTypeAnode = 0,                    /**< Anode */
    drvPinTypeCathode = !drvPinTypeAnode    /**< Cathode */
} drvPinType;

/**
 * Function configure pin like cathode or anode
 * @param pin pin data structure
 * @param type enumeration value of pin purpose
 */
static void drvSevenSegmentInitPin(drvSegmentData* pin, drvPinType type) {
    halInitGPIOStruct initStruct;
    initStruct.halPins = pin->pin;
    initStruct.halGpioSpeed = halSpeed2Mhz;
    initStruct.halPuPd = halPuPdNo;
    initStruct.halGpioMode = halModeOut;
    initStruct.halOutType = halOTPP;
    halGPIOInit(pin->port, &initStruct);
    if (type == drvPinTypeAnode)
        halGPIOResetPins(pin->port, pin->pin);
    else
        halGPIOSetPins(pin->port, pin->pin);
}

static drvSegmentData* displayCathodes;
static drvSegmentData* displayAnodes;
static uint8_t displayAnodesCount;

void drvSevenSegmentInit(drvDisplayData* data) {
    displayAnodes = data->anods;
    displayCathodes = data->cathodes;
    displayAnodesCount = data->numbersCount;
    for (uint8_t i = 0; i < data->numbersCount; i++) {
        drvSevenSegmentInitPin(&data->anods[i], drvPinTypeAnode);
    }
    for (uint8_t i = drvSegmentA; i < drvSegmentsCount; i++) {
        drvSevenSegmentInitPin(&data->cathodes[i], drvPinTypeCathode);
    }

}

static void drvSevenSegmentsSetAnode(uint8_t number) {
    halGPIOSetPins(displayAnodes[number].port,displayAnodes[number].pin);
}

static void drvSevenSegmentsResetAnode(uint8_t number) {
    halGPIOResetPins(displayAnodes[number].port,displayAnodes[number].pin);
}

static void drvSevenSegmentSetCathodes(drvSevenSegmentSymbols symbol,
        uint8_t printPoint) {
    for (uint8_t i = 0; i < drvSegmentsCount - 1; i++) {
        if ((symbol >> i) & 1 == 1)
            halGPIOResetPins(displayCathodes[i].port, displayCathodes[i].pin);
        else
            halGPIOSetPins(displayCathodes[i].port, displayCathodes[i].pin);
    }
    if (printPoint)
        halGPIOResetPins(displayCathodes[drvSegmentsCount - 1].port,
                displayCathodes[drvSegmentsCount - 1].pin);
    else
        halGPIOSetPins(displayCathodes[drvSegmentsCount - 1].port,
                displayCathodes[drvSegmentsCount - 1].pin);
}

void drvSevenSegmentPrintSymbol(drvSevenSegmentSymbols symbol,
        uint8_t symbolNum) {
    for(uint8_t i=0;i<displayAnodesCount;i++)
        drvSevenSegmentsResetAnode(i);
    drvSevenSegmentSetCathodes(symbol, 0);
    drvSevenSegmentsSetAnode(symbolNum);
}

void drvSevenSegmentClearSymbol(uint8_t symbolPos) {
    drvSevenSegmentsResetAnode(symbolPos);
}



