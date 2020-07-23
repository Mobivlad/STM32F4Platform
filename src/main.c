/**
  ******************************************************************************
  * @file    main.c
  * @author  Ac6
  * @version V1.0
  * @date    01-December-2013
  * @brief   Default main function.
  ******************************************************************************
*/

#include <stdio.h>

#include "stm32f7xx_it.h"
#include "stm32f769i_discovery_lcd.h"

#include "ul_heart_beat.h"
#include "ul_moving_average.h"
#include "bl_adc_controller.h"



#define BLINK_FREQUENCY_5_HZ 100
#define ADC_FREQUENCY_100_HZ 100

#define LCD_FRAME_BUFFER          SDRAM_DEVICE_ADDR
#define RGB565_BYTE_PER_PIXEL     2
#define ARBG8888_BYTE_PER_PIXEL   4

static void SystemClock_Config(void);

ulHeartBeatStruct      heartBeat;
ulMovingAvarege_struct adc;
blADCController_struct adcController;

int main(void)
{
    HAL_Init();

    SystemClock_Config();
    SystemCoreClockUpdate();

    heartBeat.frequency = BLINK_FREQUENCY_5_HZ;
    ulHeartBeatInit(&heartBeat);

    adc.adc = drvADC1;
    adc.frequency = ADC_FREQUENCY_100_HZ;

    ulMovingAvaregeInit(&adc);

    adcController.controlSemaphore = adc.semaphore;
    blADCControllerInit(&adcController);

    xTaskCreate(ulHeartBeatTaskFunction, "HEART_BEAT", configMINIMAL_STACK_SIZE,
                (void*) &heartBeat, 1, (xTaskHandle *) NULL);

    xTaskCreate(ulMovingAvaregeTaskFunction, "ADC", configMINIMAL_STACK_SIZE,
                    (void*) &adc, 1, (xTaskHandle *) NULL);

    xTaskCreate(ulMovingAvaregeControlFunction, "ADC_control", configMINIMAL_STACK_SIZE,
                        (void*) &adc, 1, (xTaskHandle *) NULL);

    xTaskCreate(blADCControllerTask, "BUTTON_HANDLER", configMINIMAL_STACK_SIZE,
                    (void*) &adcController, 1, (xTaskHandle *) NULL);

    vTaskStartScheduler();
}

static void SystemClock_Config(void) {
    RCC_ClkInitTypeDef RCC_ClkInitStruct;
    RCC_OscInitTypeDef RCC_OscInitStruct;

    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
    RCC_OscInitStruct.HSEState = RCC_HSE_ON;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
    RCC_OscInitStruct.PLL.PLLM = 25;
    RCC_OscInitStruct.PLL.PLLN = 432;
    RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
    RCC_OscInitStruct.PLL.PLLQ = 9;
    RCC_OscInitStruct.PLL.PLLR = 7;

    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) Error_Handler();

    RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1
            | RCC_CLOCKTYPE_PCLK2);
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_7) != HAL_OK) Error_Handler();
}
