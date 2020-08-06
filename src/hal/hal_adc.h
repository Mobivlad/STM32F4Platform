/*
 * hal_adc.h
 *
 *  Created on: 15 лип. 2020 р.
 *      Author: vladyslav.daliavskyi
 */

#ifndef HAL_HAL_ADC_H_
#define HAL_HAL_ADC_H_

#include "stm32f7xx.h"
#include "FreeRTOS.h"
#include "queue.h"
#include "task.h"
#include "semphr.h"

#define UINT16_MAX_VALUE        0xFF
#define CHANNEL_RANK            1
#define CONVERTATIONS_NUMBER    1

#define EVENTS_ON_PERIOD        1000
#define TIMER_PRESCALER         13499
#define CONV_TIMEOUT            0xFF

#define ADC_IRQn_PRIORITY       6
#define ADC_IRQn_SUB_PRIORITY   7

typedef enum {
    halADC1,
    halADC2,
    halADC3,
    halADC_Count
} halADC;

typedef enum {
    halADC_Channel0,
    halADC_Channel1,
    halADC_Channel2,
    halADC_Channel3,
    halADC_Channel4,
    halADC_Channel5,
    halADC_Channel6,
    halADC_Channel7,
    halADC_Channel8,
    halADC_Channel9,
    halADC_Channel10,
    halADC_Channel11,
    halADC_Channel12,
    halADC_Channel13,
    halADC_Channel14,
    halADC_Channel15,
    halADC_ChannelCount
} halADC_channel;

typedef struct {
    GPIO_TypeDef *          port;
    uint32_t                port_rcc;
    uint16_t                pin;
} halADC_pin_def_t;

typedef struct {
    ADC_TypeDef *           adc;
    uint32_t                adc_rcc;
    halADC_pin_def_t        adcPins[halADC_ChannelCount];
} halADC_def_t;

typedef struct {
    TIM_TypeDef *           timer;
    uint32_t                timer_rcc;
    uint32_t                trigger_value;
} halADC_timerDef_t;


typedef enum {
    halADC_clockDiv2        = ADC_CLOCK_SYNC_PCLK_DIV2,
    halADC_clockDiv4        = ADC_CLOCK_SYNC_PCLK_DIV4,
    halADC_clockDiv6        = ADC_CLOCK_SYNC_PCLK_DIV6,
    halADC_clockDiv8        = ADC_CLOCK_SYNC_PCLK_DIV8
} halADC_clockPrescaler;

typedef enum {
    halADC_dataSize6b       = ADC_RESOLUTION_6B,
    halADC_dataSize8b       = ADC_RESOLUTION_8B,
    halADC_dataSize10b      = ADC_RESOLUTION_10B,
    halADC_dataSize12b      = ADC_RESOLUTION_12B
} halADC_dataSize;

typedef enum {
    halADC_dataAlignleft    = ADC_DATAALIGN_LEFT,
    halADC_dataAlignRight   = ADC_DATAALIGN_RIGHT
} halADC_dataAlign;

typedef enum {
    halADC_continuousConvMode = 0,
    halADC_singleConvMode,
    halADC_triggerConvMode
} halADC_convertationMode;

typedef enum {
    halADC_timer2,
    halADC_timer4,
    halADC_timer5,
    halADC_timer6,
    halADC_timersCount
} halADC_triggerTimer;

typedef struct {
    halADC_triggerTimer     timer;

    uint32_t                frequency;
} halADC_timerInitStruct;

typedef struct {
    halADC                  adc;
    halADC_channel          channel;

    halADC_clockPrescaler   clockPrescaler;
    halADC_convertationMode convMode;
    halADC_dataSize         dataSize;
    halADC_dataAlign        dataAlign;

    halADC_timerInitStruct  triggerTimerInitStruct;
    QueueHandle_t           queue;
} halADC_initStruct;

typedef struct {
    ADC_HandleTypeDef       adcDef;
    TIM_HandleTypeDef       timer;

    halADC                  adc;
    halADC_channel          channel;

    halADC_convertationMode mode;
    QueueHandle_t           queue;
    SemaphoreHandle_t       semaphore;
} halADC_struct;


/**
 * Initial function for ADC
 * @param adcStruct pointer on ADC state structure
 * @param adcInitStruct pointer on structure with initial parameters
 */
void halADCInit(halADC_struct* adcStruct, const halADC_initStruct* adcInitStruct);

/**
 * Start ADC function
 * @param adcStruct pointer on ADC state structure
 */
void halADCStart(halADC_struct* adcStruct);

/**
 * Stop ADC function
 * @param adcStruct pointer on ADC state structure
 */
void halADCStop(halADC_struct* adcStruct);

/**
 * Get currently ADC value
 * @param adcStruct pointer on ADC state structure
 */
uint16_t halADCGetValue(halADC_struct* adcStruct);

#endif /* HAL_HAL_ADC_H_ */
