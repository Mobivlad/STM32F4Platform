/*
 * drv_adc.h
 *
 *  Created on: 21 лип. 2020 р.
 *      Author: vladyslav.daliavskyi
 */

#ifndef DRV_DRV_ADC_H_
#define DRV_DRV_ADC_H_

#include "hal_adc.h"

typedef enum {
    drvADC1,
    drvADC_Count
} drvADC;

typedef struct {
    halADC              adc;
    halADC_channel      channel;
    halADC_dataSize     dataSize;

    halADC_triggerTimer timer;
} drvADC_def_t;

typedef struct {
    halADC_struct       halADC;
} drvADC_struct;

/**
 * Initial function for ADC
 * @param adc_struct pointer on ADC state structure
 * @param adc name of ADC
 * @param frequency count of ADC measurements on 1 s
 * @param queue queue buffer for results
 * @param semaphore control semaphore
 */
void drvADCInit(drvADC_struct* adcStruct, drvADC adc, uint32_t frequency, QueueHandle_t queue, SemaphoreHandle_t semaphore);

/**
 * Start ADC function
 * @param adcStruct pointer on ADC state structure
 */
void drvADCStart(drvADC_struct* adcStruct);

/**
 * Stop ADC function
 * @param adcStruct pointer on ADC state structure
 */
void drvADCStop(drvADC_struct* adcStruct);

/**
 * Get currently ADC value
 * @param adcStruct pointer on ADC state structure
 */
void drvADCGetVal(drvADC_struct* adcStruct);

#endif /* DRV_DRV_ADC_H_ */
