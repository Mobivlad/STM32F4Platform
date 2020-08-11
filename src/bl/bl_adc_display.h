/*
 * bl_adc_display.h
 *
 *  Created on: Aug 7, 2020
 *      Author: Sigma
 */

#ifndef BL_BL_ADC_DISPLAY_H_
#define BL_BL_ADC_DISPLAY_H_

#include "FreeRTOS.h"
#include "semphr.h"

#include "stm32f769i_discovery.h"
#include "stm32f769i_discovery_lcd.h"

#define LCD_FRAME_BUFFER          SDRAM_DEVICE_ADDR
#define RGB565_BYTE_PER_PIXEL     2
#define ARBG8888_BYTE_PER_PIXEL   4

#define DISPLAY_NAME        "DIGITAL VOLTMETER"
#define TEXT_Y_POS          10

#define UIN16_MAX           0xFFFF

#define MAX_ADC_VAL         0x0FFF

#define DISPLAY_HEIGHT      480
#define DISPLAY_WIDTH       800

#define GRAPH_TOP_MARGIN    40
#define GRAPH_MARGIN        10

#define GRAPH_HEIGHT        (DISPLAY_HEIGHT - GRAPH_MARGIN - GRAPH_TOP_MARGIN)
#define GRAPH_WIDTH         (DISPLAY_WIDTH - 2 * GRAPH_MARGIN)

#define GRAPH_PADDING       5
#define AREA_HEIGHT         (GRAPH_HEIGHT - 2 * GRAPH_PADDING)

#define START_X             GRAPH_MARGIN
#define END_X               DISPLAY_WIDTH - GRAPH_MARGIN

#define WIDTH_STEP          5

typedef enum {
    blADCDisplay_OK = 0,
    blADCDisplay_EROOR,
    blADCDisplay_NULL_POINT
} blADCDisplay_error;

typedef struct {
    QueueHandle_t       adcValues;

    uint16_t            currentX;
    uint16_t            currentY;
    uint16_t            lastY;
} blADCDisplay_struct;

blADCDisplay_error blADCDisplayInit(blADCDisplay_struct* displayStruct, QueueHandle_t queueValues);

void blADCDisplayTask(void* parametr);

blADCDisplay_error blADCDisplayStart(blADCDisplay_struct* displayStruct);
blADCDisplay_error blADCDisplayStop(blADCDisplay_struct* displayStruct);


#endif /* BL_BL_ADC_DISPLAY_H_ */
