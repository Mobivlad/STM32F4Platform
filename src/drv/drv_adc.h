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

void drvADCInit(drvADC_struct* adc_struct, drvADC adc, uint32_t frequency, QueueHandle_t* queue);

void drvADCStart(drvADC_struct* adc_struct);

void drvADCStop(drvADC_struct* adc_struct);

void drvADCGetVal(drvADC_struct* adc_struct);

#endif /* DRV_DRV_ADC_H_ */
