#include "ul_heart_beat.h"
#include "ul_moving_average.h"
#include "drv_usb_cdc.h"

#define BLINK_FREQUENCY_5_HZ 1000

ulHeartBeatStruct heartBeat;
drvUSBCDC_struct usb;

static void SystemClock_Config(void);

void initialFunction(void* parametr) {
  ulHeartBeatInit(&heartBeat, BLINK_FREQUENCY_5_HZ);

  drvUSBCDCInit(&usb, NULL);
 
  xTaskCreate(ulHeartBeatTaskFunction, "HEART_BEAT", configMINIMAL_STACK_SIZE, (void*) &heartBeat,
            1, (xTaskHandle *) NULL);

  while(1) {
    drvUSBCDCWriteStr(&usb, "HELLO\r\n");
    vTaskDelay(1000);
  }
}

int main(void)
{
  HAL_Init();

  SystemClock_Config();
  SystemCoreClockUpdate();
  
  xTaskCreate(initialFunction, "INITIAL TASK", configMINIMAL_STACK_SIZE, (void*) NULL,
            1, (xTaskHandle *) NULL); 

  vTaskStartScheduler();
}

static void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 168;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 7;
  HAL_RCC_OscConfig(&RCC_OscInitStruct);
  
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;
  HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5);
}