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
    memset(displayStruct->Y, UIN16_MAX, STEPS_COUNT * sizeof(uint16_t));
    displayStruct->stepIndex = 0;

    return blADCDisplay_OK;
}

blADCDisplay_error blADCDisplayStop(blADCDisplay_struct* displayStruct) {
    if (displayStruct == NULL) {
        return blADCDisplay_NULL_POINT;
    }
    // @TODO check if DisplayStop is needed
    return blADCDisplay_OK;
}

void blADCDisplaySDStatus(blADCDisplay_SDStatus sdStatus) {
    BSP_LCD_SetFont(&Font16);
    BSP_LCD_SetTextColor(sdStatus == blADCDisplay_SD_OK ? LCD_COLOR_GREEN : LCD_COLOR_RED);
    BSP_LCD_DisplayStringAt(0, TEXT_Y_POS,
            sdStatus == blADCDisplay_SD_OK ? (uint8_t*) SD_OK_TEXT : (uint8_t*) SD_ERROR_TEXT,
            RIGHT_MODE);
}

static void redrawAll(blADCDisplay_struct* displayStruct) {
    uint16_t next_index = (displayStruct->stepIndex + 1) % STEPS_COUNT;

    for (uint16_t i = 0; i < STEPS_COUNT-1; i++) {
        uint16_t pointIndex = (next_index + i) % STEPS_COUNT;
        uint16_t nextPointIndex = (pointIndex == STEPS_COUNT-1) ? (0) : pointIndex+1;
        BSP_LCD_DrawLine(START_X + (i) * WIDTH_STEP+1,
                    DISPLAY_HEIGHT - GRAPH_PADDING - displayStruct->Y[pointIndex] - GRAPH_MARGIN,
                            START_X + (i+1) * WIDTH_STEP+1,
                            DISPLAY_HEIGHT - GRAPH_PADDING - displayStruct->Y[nextPointIndex] - GRAPH_MARGIN);
    }
}

static void drawGraph(blADCDisplay_struct* displayStruct, uint16_t newVal) {
    uint16_t index = displayStruct->stepIndex;
    //uint16_t pr_index = (displayStruct->stepIndex + STEPS_COUNT - 1) % STEPS_COUNT;
    uint16_t next_index = (displayStruct->stepIndex + 1) % STEPS_COUNT;
    if (displayStruct->Y[next_index] == UIN16_MAX) {
        displayStruct->Y[index] = newVal;
        if (index == 0) {
            displayStruct->stepIndex = (displayStruct->stepIndex + 1) % STEPS_COUNT;
            displayStruct->Y[displayStruct->stepIndex] = newVal;
            return;
        } else {
            displayStruct->stepIndex = (displayStruct->stepIndex + 1) % STEPS_COUNT;
            displayStruct->Y[displayStruct->stepIndex] = newVal;
            BSP_LCD_DrawLine(
                    START_X + (index - 1) * WIDTH_STEP+1,
                    DISPLAY_HEIGHT - GRAPH_PADDING - displayStruct->Y[index - 1] - GRAPH_MARGIN,
                    START_X + index * WIDTH_STEP+1,
                    DISPLAY_HEIGHT - GRAPH_PADDING - displayStruct->Y[index] - GRAPH_MARGIN);
        }
    } else {
        BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
        redrawAll(displayStruct);
        displayStruct->stepIndex = (displayStruct->stepIndex + 1) % STEPS_COUNT;
        displayStruct->Y[displayStruct->stepIndex] = newVal;
        BSP_LCD_SetTextColor(LCD_COLOR_RED);
        redrawAll(displayStruct);
    }
}

void blADCDisplayTask(void* parametr) {
    blADCDisplay_struct* displayStruct = (blADCDisplay_struct*) parametr;
    while (1) {
        uint16_t adcValue;
        xQueueReceive(displayStruct->adcValues, &adcValue, portMAX_DELAY);

        //displayStruct->Y[displayStruct->stepIndex] = adcValue * AREA_HEIGHT / MAX_ADC_VAL;
        drawGraph(displayStruct, adcValue * AREA_HEIGHT / MAX_ADC_VAL);
        /*displayStruct->currentX += WIDTH_STEP;
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
        */
    }
}
