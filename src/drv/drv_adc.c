/*
 * drv_adc.c
 *
 *  Created on: 21 лип. 2020 р.
 *      Author: vladyslav.daliavskyi
 */

#include "drv_adc.h"

static const drvADC_def_t drvADC_def[drvADC_Count] = {
        {
                .adc = halADC1,
                .channel = halADC_Channel4,
                .dataSize = halADC_dataSize12b,
                .timer = halADC_timer2
        }
};

void drvADCInit(drvADC_struct* adc_struct, drvADC adc, uint32_t frequency, QueueHandle_t* queue) {

    halADC_initStruct initStruct;

    initStruct.adc                              = drvADC_def[adc].adc;
    initStruct.channel                          = drvADC_def[adc].channel;
    initStruct.dataSize                         = drvADC_def[adc].dataSize;
    initStruct.triggerTimerInitStruct.timer     = drvADC_def[adc].timer;
    initStruct.triggerTimerInitStruct.frequency = frequency;
    initStruct.queue                            = queue;

    initStruct.clockPrescaler                   = halADC_clockDiv4;
    initStruct.convMode                         = halADC_triggerConvMode;
    initStruct.dataAlign                        = halADC_dataAlignRight;

    halADCInit((halADC_struct*)adc_struct, &initStruct);
}


void drvADCStart(drvADC_struct* adc_struct) {
    halADCStart((halADC_struct*)adc_struct);
}

void drvADCStop(drvADC_struct* adc_struct) {
    halADCStop((halADC_struct*)adc_struct);
}

void drvADCGetValue(drvADC_struct* adc_struct) {
    halADCGetValue((halADC_struct*)adc_struct);
}
