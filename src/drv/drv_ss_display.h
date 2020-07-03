/*
 * drv_ss_display.h
 *
 *  Created on: 3 лип. 2020 р.
 *      Author: vladyslav.daliavskyi
 */

#ifndef DRV_DRV_SS_DISPLAY_H_
#define DRV_DRV_SS_DISPLAY_H_

#include "hal_gpio.h"
#include "stddef.h"

#define DISPLAY_NUMBER_COUNT    3
#define DISPLAY_SEGMENT_COUNT   8
#define LIBRARY_CAPASITY        11

#define TIMER_CLOCK_PRESCULAR   5249
#define DISPLAY_UPDATE_FREQ     50

typedef enum {
    seg_a  = 0x01,  // Segment A
    seg_b  = 0x02,  // Segment B
    seg_c  = 0x04,  // Segment C
    seg_d  = 0x08,  // Segment D
    seg_e  = 0x10,  // Segment E
    seg_f  = 0x20,  // Segment F
    seg_g  = 0x40,  // Segment G
    seg_dp = 0x80   // Point segment
} drvSSDislay_anodes;

typedef struct {
    char    character;
    uint8_t symbol_def;  // value that shows symbol segments
} drvSSDisplay_SymbolPresentation;

typedef struct {
    const halGPIO_port  port;

    const halGPIO_pin   anodes[DISPLAY_SEGMENT_COUNT];
    const halGPIO_pin   cathodes[DISPLAY_NUMBER_COUNT];

    uint16_t            outValues[DISPLAY_NUMBER_COUNT];     // array for DMA

} drvSSDislay_struct;

/**
 *  Initial function for work with 7segment display
 */
void drvSSDisplayInit();

/***
 *  Function changes the display number
 *  @param value new number
 */
void drvSSDisplaySetValue(uint8_t value);

#endif /* DRV_DRV_SS_DISPLAY_H_ */
