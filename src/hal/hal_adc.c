/*
 * hal_adc.c
 *
 *  Created on: 15 лип. 2020 р.
 *      Author: vladyslav.daliavskyi
 */

#include "hal_adc.h"

static const halADC_timerDef timer_def[halADC_timersCount] =
{
        {   TIM2, RCC_APB1ENR_TIM2EN, ADC_EXTERNALTRIGCONV_T2_TRGO    },
        {   TIM4, RCC_APB1ENR_TIM4EN, ADC_EXTERNALTRIGCONV_T4_TRGO    },
        {   TIM5, RCC_APB1ENR_TIM5EN, ADC_EXTERNALTRIGCONV_T5_TRGO    },
        {   TIM6, RCC_APB1ENR_TIM6EN, ADC_EXTERNALTRIGCONV_T6_TRGO    }
};

static const halADC_def adc_def[halADC_Count] =
{
        {
                ADC1,   RCC_APB2ENR_ADC1EN,
                {
                        {GPIOA, RCC_AHB1ENR_GPIOAEN, GPIO_PIN_0},
                        {GPIOA, RCC_AHB1ENR_GPIOAEN, GPIO_PIN_1},
                        {GPIOA, RCC_AHB1ENR_GPIOAEN, GPIO_PIN_2},
                        {GPIOA, RCC_AHB1ENR_GPIOAEN, GPIO_PIN_3},
                        {GPIOA, RCC_AHB1ENR_GPIOAEN, GPIO_PIN_4},
                        {GPIOA, RCC_AHB1ENR_GPIOAEN, GPIO_PIN_5},
                        {GPIOA, RCC_AHB1ENR_GPIOAEN, GPIO_PIN_6},
                        {GPIOA, RCC_AHB1ENR_GPIOAEN, GPIO_PIN_7},
                        {GPIOB, RCC_AHB1ENR_GPIOBEN, GPIO_PIN_0},
                        {GPIOB, RCC_AHB1ENR_GPIOBEN, GPIO_PIN_1},
                        {GPIOC, RCC_AHB1ENR_GPIOCEN, GPIO_PIN_0},
                        {GPIOC, RCC_AHB1ENR_GPIOCEN, GPIO_PIN_1},
                        {GPIOC, RCC_AHB1ENR_GPIOCEN, GPIO_PIN_2},
                        {GPIOC, RCC_AHB1ENR_GPIOCEN, GPIO_PIN_3},
                        {GPIOC, RCC_AHB1ENR_GPIOCEN, GPIO_PIN_4},
                        {GPIOC, RCC_AHB1ENR_GPIOCEN, GPIO_PIN_5}
                }
        },
        {
                ADC2,   RCC_APB2ENR_ADC1EN,
                {
                        {GPIOA, RCC_AHB1ENR_GPIOAEN, GPIO_PIN_0},
                        {GPIOA, RCC_AHB1ENR_GPIOAEN, GPIO_PIN_1},
                        {GPIOA, RCC_AHB1ENR_GPIOAEN, GPIO_PIN_2},
                        {GPIOA, RCC_AHB1ENR_GPIOAEN, GPIO_PIN_3},
                        {GPIOA, RCC_AHB1ENR_GPIOAEN, GPIO_PIN_4},
                        {GPIOA, RCC_AHB1ENR_GPIOAEN, GPIO_PIN_5},
                        {GPIOA, RCC_AHB1ENR_GPIOAEN, GPIO_PIN_6},
                        {GPIOA, RCC_AHB1ENR_GPIOAEN, GPIO_PIN_7},
                        {GPIOB, RCC_AHB1ENR_GPIOBEN, GPIO_PIN_0},
                        {GPIOB, RCC_AHB1ENR_GPIOBEN, GPIO_PIN_1},
                        {GPIOC, RCC_AHB1ENR_GPIOCEN, GPIO_PIN_0},
                        {GPIOC, RCC_AHB1ENR_GPIOCEN, GPIO_PIN_1},
                        {GPIOC, RCC_AHB1ENR_GPIOCEN, GPIO_PIN_2},
                        {GPIOC, RCC_AHB1ENR_GPIOCEN, GPIO_PIN_3},
                        {GPIOC, RCC_AHB1ENR_GPIOCEN, GPIO_PIN_4},
                        {GPIOC, RCC_AHB1ENR_GPIOCEN, GPIO_PIN_5}
                }
        },
        {
                ADC3,   RCC_APB2ENR_ADC1EN,
                {
                        {GPIOA, RCC_AHB1ENR_GPIOAEN, GPIO_PIN_0},
                        {GPIOA, RCC_AHB1ENR_GPIOAEN, GPIO_PIN_1},
                        {GPIOA, RCC_AHB1ENR_GPIOAEN, GPIO_PIN_2},
                        {GPIOA, RCC_AHB1ENR_GPIOAEN, GPIO_PIN_3},
                        {GPIOF, RCC_AHB1ENR_GPIOFEN, GPIO_PIN_6},
                        {GPIOF, RCC_AHB1ENR_GPIOFEN, GPIO_PIN_7},
                        {GPIOF, RCC_AHB1ENR_GPIOFEN, GPIO_PIN_8},
                        {GPIOF, RCC_AHB1ENR_GPIOFEN, GPIO_PIN_9},
                        {GPIOF, RCC_AHB1ENR_GPIOFEN, GPIO_PIN_10},
                        {GPIOF, RCC_AHB1ENR_GPIOFEN, GPIO_PIN_3},
                        {GPIOC, RCC_AHB1ENR_GPIOCEN, GPIO_PIN_0},
                        {GPIOC, RCC_AHB1ENR_GPIOCEN, GPIO_PIN_1},
                        {GPIOC, RCC_AHB1ENR_GPIOCEN, GPIO_PIN_2},
                        {GPIOC, RCC_AHB1ENR_GPIOCEN, GPIO_PIN_3},
                        {GPIOF, RCC_AHB1ENR_GPIOFEN, GPIO_PIN_4},
                        {GPIOF, RCC_AHB1ENR_GPIOFEN, GPIO_PIN_5}
                }
        }
};

static const uint32_t channelValues[halADC_ChannelCount] = {
        ADC_CHANNEL_0,  ADC_CHANNEL_1,  ADC_CHANNEL_2,  ADC_CHANNEL_3,
        ADC_CHANNEL_4,  ADC_CHANNEL_5,  ADC_CHANNEL_6,  ADC_CHANNEL_7,
        ADC_CHANNEL_8,  ADC_CHANNEL_9,  ADC_CHANNEL_10, ADC_CHANNEL_11,
        ADC_CHANNEL_12, ADC_CHANNEL_13, ADC_CHANNEL_14, ADC_CHANNEL_15
};

static halADC_struct* initedADC[halADC_Count] = { NULL };

void halADCInit(halADC_struct* adcStruct, halADC_initStruct* adcInitStruct) {
    // enable RCC
    uint8_t adcId = adcInitStruct->adc;
    SET_BIT(RCC->APB2ENR, adc_def[adcId].adc_rcc);
    SET_BIT(RCC->AHB1ENR, adc_def[adcId].adcPins[adcInitStruct->channel].port_rcc);

    adcStruct->queue = adcInitStruct->queue;

    GPIO_InitTypeDef    pinInitStruct;
    pinInitStruct.Mode      = GPIO_MODE_ANALOG;
    pinInitStruct.Pin       = adc_def[adcId].adcPins[adcInitStruct->channel].pin;
    pinInitStruct.Pull      = GPIO_NOPULL;
    pinInitStruct.Speed     = GPIO_SPEED_FREQ_LOW;

    HAL_GPIO_Init(adc_def[adcId].adcPins[adcInitStruct->channel].port, &pinInitStruct);

    ADC_HandleTypeDef* adc = (ADC_HandleTypeDef*)adcStruct;
    adc->Instance                   = adc_def[adcId].adc;
    adc->Init.ClockPrescaler        = adcInitStruct->clockPrescaler;

    adc->Init.Resolution            = adcInitStruct->dataSize;
    adc->Init.NbrOfConversion       = CONVERTATIONS_NUMBER;
    adc->Init.DataAlign             = adcInitStruct->dataAlign;
    adc->Init.ScanConvMode          = DISABLE;
    adc->Init.DiscontinuousConvMode = DISABLE;
    adc->Init.DMAContinuousRequests = DISABLE;
    adc->Init.EOCSelection          = ADC_EOC_SINGLE_CONV;

    adc->Init.ContinuousConvMode    =
                adcInitStruct->convMode == halADC_continuousConvMode ? ENABLE : DISABLE;

    if (adcInitStruct->convMode == halADC_triggerConvMode) {

        adc->Init.ExternalTrigConv =
                timer_def[adcInitStruct->triggerTimerInitStruct.timer].trigger_value;
        adc->Init.ExternalTrigConvEdge  = ADC_EXTERNALTRIGCONVEDGE_RISING;

        SET_BIT(RCC->APB1ENR, timer_def[adcInitStruct->triggerTimerInitStruct.timer].timer_rcc);

        adcStruct->timer.Instance   = timer_def[adcInitStruct->triggerTimerInitStruct.timer].timer;
        TIM_Base_InitTypeDef* timer = &(adcStruct->timer.Init);

        timer->Prescaler            = TIMER_PRESCALER;
        timer->CounterMode          = TIM_COUNTERMODE_UP;
        timer->Period               = EVENTS_ON_PERIOD / adcInitStruct->triggerTimerInitStruct.frequency;
        timer->ClockDivision        = TIM_CLOCKDIVISION_DIV4;

        timer->AutoReloadPreload    = TIM_AUTORELOAD_PRELOAD_DISABLE;

        HAL_TIM_Base_Init(&(adcStruct->timer));

        TIM_MasterConfigTypeDef sMastrConfig = { 0 };
        sMastrConfig.MasterOutputTrigger = TIM_TRGO_UPDATE;
        HAL_TIMEx_MasterConfigSynchronization(&(adcStruct->timer), &sMastrConfig);
    } else {

        adc->Init.ExternalTrigConvEdge  = ADC_EXTERNALTRIGCONVEDGE_NONE;

    }
    HAL_ADC_Init(adc);



    adcStruct->mode = adcInitStruct->convMode;

    ADC_ChannelConfTypeDef channelConfig;
    channelConfig.Channel = channelValues[adcInitStruct->channel];
    channelConfig.Rank = CHANNEL_RANK;
    channelConfig.SamplingTime = ADC_SAMPLETIME_3CYCLES;

    HAL_ADC_ConfigChannel(adc, &channelConfig);

    HAL_NVIC_SetPriority(ADC_IRQn, ADC_IRQn_PRIORITY, ADC_IRQn_SUB_PRIORITY);
    HAL_NVIC_EnableIRQ(ADC_IRQn);

    initedADC[adcStruct->adc] = adcStruct;
}

void halADCStart(halADC_struct* adcStruct) {
    HAL_ADC_Start_IT((ADC_HandleTypeDef*)adcStruct);
    if (adcStruct->mode == halADC_triggerConvMode) {
        HAL_TIM_Base_Start(&(adcStruct->timer));
    }
}

void halADCStop(halADC_struct* adcStruct) {
    HAL_ADC_Stop_IT((ADC_HandleTypeDef*)adcStruct);
    if (adcStruct->mode == halADC_triggerConvMode) {
        HAL_TIM_Base_Stop(&(adcStruct->timer));
    }
}

uint16_t halADCGetValue(halADC_struct* adcStruct) {
    if (adcStruct->mode == halADC_singleConvMode) {
        HAL_ADC_PollForConversion((ADC_HandleTypeDef*)adcStruct, CONV_TIMEOUT);
    }
    return HAL_ADC_GetValue((ADC_HandleTypeDef*)adcStruct);
}

void ADC_IRQHandler(void) {
    for (uint8_t i = 0; i < halADC_Count; i++) {
        HAL_ADC_IRQHandler(&(initedADC[i]->adcDef));
    }
}

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc) {
    BaseType_t res;
    for (uint8_t i=0; i < halADC_Count; i++) {
        if (adc_def[i].adc == hadc->Instance) {
            const uint16_t value = HAL_ADC_GetValue(hadc);
            res = xQueueSendFromISR(*(initedADC[i]->queue), (const void* )&value, NULL);
        }
    }
}
