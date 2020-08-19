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

#include "stm32f7xx_hal.h"
#include "stm32f7xx_it.h"

#include "ul_heart_beat.h"
#include "bl_adc_controller.h"

#define BLINK_FREQUENCY_5_HZ 100
#define ADC_FREQUENCY 100

static void SystemClock_Config(void);

void initialTask();
void initSystemView();

int main(void)
{
    HAL_Init();

    SystemClock_Config();
    SystemCoreClockUpdate();

    initSystemView();

    xTaskCreate(initialTask, "Initial task", (uint16_t) 4 * configMINIMAL_STACK_SIZE, NULL, 2,
            (xTaskHandle *) NULL);

    vTaskStartScheduler();
}

void initSystemView() {
    DWT->CTRL |= (1<<0);

    SEGGER_SYSVIEW_Conf();
    vSetVarulMaxPRIGROUPValue();
    SEGGER_SYSVIEW_Start();
}

ulHeartBeatStruct       heartBeat;

blADCController_struct  adcController;

blADCDisplay_struct     adcd;
blADCFileWriter_struct  adcw;

void initialTask() {

    blADCFW_error initRes = blADCFileWriterInit(&adcw);

    blADCDisplayInit(&adcd, initRes);

    blADCControllerInit(&adcController, ADC_FREQUENCY, adcd.displayDataQueue, adcw.fileWriterDataQueue);

    ulHeartBeatInit(&heartBeat, BLINK_FREQUENCY_5_HZ);

    xTaskCreate(ulHeartBeatTaskFunction, "HEART_BEAT", configMINIMAL_STACK_SIZE, (void*) &heartBeat,
            1, (xTaskHandle *) NULL);

    xTaskCreate(ulMovingAvaregeControlTask, "ADC_FILTER_CONTROLLER", configMINIMAL_STACK_SIZE,
            (void*) &adcController.movingAvarageUtil, 1, (xTaskHandle *) NULL);

    xTaskCreate(ulMovingAvaregeCalculateTask, "ADC_FILTER_CALCULATOR", configMINIMAL_STACK_SIZE,
            (void*) &adcController.movingAvarageUtil, 1, (xTaskHandle *) NULL);

    xTaskCreate(blADCControllerButtonTask, "BUTTON_HANDLER", configMINIMAL_STACK_SIZE,
            (void*) &adcController, 1, (xTaskHandle *) NULL);

    xTaskCreate(blADCControllerSwitchTask, "ADC_SWITCH_HANDLER", configMINIMAL_STACK_SIZE,
            (void*) &adcController, 1, (xTaskHandle *) NULL);

    if (initRes == blADCFW_OK) {
        xTaskCreate(blADCFileWriterTask, "ADC_WRITER", configMINIMAL_STACK_SIZE, (void*) &adcw, 1,
                (xTaskHandle *) NULL);
    }

    xTaskCreate(blADCDisplayTask, "ADC_DISPLAY", configMINIMAL_STACK_SIZE, (void*) &adcd, 1,
            (xTaskHandle *) NULL);
    while(1) {
        vTaskDelay(portMAX_DELAY);
    }
}

static void SystemClock_Config(void) {
    RCC_OscInitTypeDef RCC_OscInitStruct = { 0 };
    RCC_ClkInitTypeDef RCC_ClkInitStruct = { 0 };
    RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = { 0 };

    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
    RCC_OscInitStruct.HSEState = RCC_HSE_ON;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
    RCC_OscInitStruct.PLL.PLLM = 25;
    RCC_OscInitStruct.PLL.PLLN = 432;
    RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
    RCC_OscInitStruct.PLL.PLLQ = 9;
    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) Error_Handler();

    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1
            | RCC_CLOCKTYPE_PCLK2;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_7) != HAL_OK) Error_Handler();
    PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_SDMMC2 | RCC_PERIPHCLK_CLK48;
    PeriphClkInitStruct.Clk48ClockSelection = RCC_CLK48SOURCE_PLL;
    PeriphClkInitStruct.Sdmmc2ClockSelection = RCC_SDMMC2CLKSOURCE_CLK48;
    if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK) Error_Handler();
}
