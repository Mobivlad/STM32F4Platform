/*
 * drv_seven_segment.h
 *
 *  Created on: 3 черв. 2020 р.
 *      Author: vladyslav.daliavskyi
 */

#ifndef DRV_DRV_SEVEN_SEGMENT_H_
#define DRV_DRV_SEVEN_SEGMENT_H_

#include "hal_gpio.h"

/// Port's pin enumeration
typedef enum {
    drvSegmentPin0 = halPin_0,       /**< Pin 0 */
    drvSegmentPin1 = halPin_1,       /**< Pin 1 */
    drvSegmentPin2 = halPin_2,       /**< Pin 2 */
    drvSegmentPin3 = halPin_3,       /**< Pin 3 */
    drvSegmentPin4 = halPin_4,       /**< Pin 4 */
    drvSegmentPin5 = halPin_5,       /**< Pin 5 */
    drvSegmentPin6 = halPin_6,       /**< Pin 6 */
    drvSegmentPin7 = halPin_7,       /**< Pin 7 */
    drvSegmentPin8 = halPin_8,       /**< Pin 8 */
    drvSegmentPin9 = halPin_9,       /**< Pin 9 */
    drvSegmentPin10 = halPin_10,     /**< Pin 10 */
    drvSegmentPin11 = halPin_11,     /**< Pin 11 */
    drvSegmentPin12 = halPin_12,     /**< Pin 12 */
    drvSegmentPin13 = halPin_13,     /**< Pin 13 */
    drvSegmentPin14 = halPin_14,     /**< Pin 14 */
    drvSegmentPin15 = halPin_15      /**< Pin 15 */
} drvSegmentPins;

/// STM32F407-Disk1 port enumeration
typedef enum {
    drvSegmentPortA = halPortA,   /**< Port A */
    drvSegmentPortB = halPortB,   /**< Port B */
    drvSegmentPortC = halPortC,   /**< Port C */
    drvSegmentPortD = halPortD,   /**< Port D */
    drvSegmentPortE = halPortE,   /**< Port E */
} drvSegmentPort;

/// Display symbols enumeration
typedef enum {
    zero = 0x3F,
    one = 0x06,
    two = 0x5B,
    three = 0x4F,
    four = 0x66,
    five = 0x6D,
    six = 0x7D,
    seven = 0x03,
    eight = 0x7F,
    nine = 0x6F
} drvSevenSegmentNumbers;

/// Segment data structure
typedef struct {
    drvSegmentPins pin;      /**< segment pin */
    drvSegmentPort port;     /**< segment port */
} drvSegmentData;

/// Cathodes data structure
typedef enum {
    drvSegmentA = 0,                  /**< A segment */
    drvSegmentB,                      /**< B segment */
    drvSegmentC,                      /**< C segment */
    drvSegmentD,                      /**< D segment */
    drvSegmentE,                      /**< E segment */
    drvSegmentF,                      /**< F segment */
    drvSegmentG,                      /**< G segment */
    drvSegmentDP,                     /**< Point segment */
    drvSegmentsCount        /**< Segments count */
} drvNumberCathodesData;

/// Display data structure
typedef struct {
    drvSegmentData* cathodes;           /**< Pointer on cathode pointers array */
    drvSegmentData* anods;              /**< Pointer on numbers anode pointers array */
    uint8_t numbersCount;               /**< Count of anodes */
} drvDisplayData;

/**
 * Initial function for 7-segments display
 * @param data structure of display data
 */
void drvSevenSegmentInit(drvDisplayData* data);

/**
 * Switch off number displaying
 * @param numberPos order position of display symbol
 */
void drvSevenSegmentClearNumber(uint8_t numberPos);

/**
 * Display number symbol function
 * @param number display symbol
 * @param numberPos order position of display symbol
 */
void drvSevenSegmentPrintNumber(drvSevenSegmentNumbers number, uint8_t numberPos);

#endif /* DRV_DRV_SEVEN_SEGMENT_H_ */
