/*
 * bl_adc_display.c
 *
 *  Created on: Aug 7, 2020
 *      Author: Sigma
 */

#include "bl_adc_display.h"

static void clearArea() {

    // clear graph area
    BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
    BSP_LCD_FillRect(GRAPH_MARGIN, GRAPH_TOP_MARGIN, GRAPH_WIDTH, GRAPH_HEIGHT);
    BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
    BSP_LCD_DrawRect(GRAPH_MARGIN, GRAPH_TOP_MARGIN, GRAPH_WIDTH, GRAPH_HEIGHT);
    // draw graph borders

    BSP_LCD_SetTextColor(LCD_COLOR_RED);
}

blADCDisplay_error blADCDisplayInit(blADCDisplay_struct* displayStruct, QueueHandle_t queueValues) {
    if (displayStruct == NULL || queueValues == NULL) {
        return blADCDisplay_NULL_POINT;
    }

    displayStruct->adcValues = queueValues;

    if (BSP_LCD_Init() != 0) {
        return blADCDisplay_EROOR;
    }

    BSP_LCD_LayerDefaultInit(LTDC_ACTIVE_LAYER_BACKGROUND, LCD_FRAME_BUFFER);
    BSP_LCD_SetLayerVisible(LTDC_ACTIVE_LAYER_BACKGROUND, ENABLE);
    BSP_LCD_SelectLayer(LTDC_ACTIVE_LAYER_BACKGROUND);

    BSP_LCD_Clear(LCD_COLOR_WHITE);
    BSP_LCD_SetBackColor(LCD_COLOR_WHITE);
    BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
    BSP_LCD_DisplayOn();

    BSP_LCD_DisplayStringAt(0, TEXT_Y_POS, (uint8_t*) DISPLAY_NAME, CENTER_MODE);

    clearArea();

    return blADCDisplay_OK;
}

blADCDisplay_error blADCDisplayStart(blADCDisplay_struct* displayStruct) {
    if (displayStruct == NULL) {
        return blADCDisplay_NULL_POINT;
    }

    clearArea();

    // set start values
    displayStruct->currentX = START_X;
    displayStruct->currentY = UIN16_MAX;
    displayStruct->lastY    = UIN16_MAX;

    return blADCDisplay_OK;
}

blADCDisplay_error blADCDisplayStop(blADCDisplay_struct* displayStruct) {
    if (displayStruct == NULL) {
        return blADCDisplay_NULL_POINT;
    }
    // @TODO check if DisplayStop is needed
    return blADCDisplay_OK;
}

void blADCDisplayTask(void* parametr) {
    blADCDisplay_struct* displayStruct = (blADCDisplay_struct*) parametr;
    while (1) {
        displayStruct->currentX += WIDTH_STEP;
        if (displayStruct->currentX > GRAPH_WIDTH + GRAPH_MARGIN) {
            clearArea();
            displayStruct->currentX = GRAPH_MARGIN + WIDTH_STEP;
        }

        uint16_t adcValue;
        xQueueReceive(displayStruct->adcValues, &adcValue, portMAX_DELAY);

        if (displayStruct->currentY == UIN16_MAX) {
            displayStruct->currentY = adcValue * AREA_HEIGHT / MAX_ADC_VAL;
            continue;
        }
        displayStruct->lastY = displayStruct->currentY;
        displayStruct->currentY = adcValue * AREA_HEIGHT / MAX_ADC_VAL;
        BSP_LCD_DrawLine(displayStruct->currentX - WIDTH_STEP,
                DISPLAY_HEIGHT - GRAPH_PADDING - displayStruct->lastY - GRAPH_MARGIN, displayStruct->currentX,
                DISPLAY_HEIGHT - GRAPH_PADDING - displayStruct->currentY - GRAPH_MARGIN);
    }
}
