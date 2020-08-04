#include "hal_sdio.h"

static const halSDIO_def_t sdio_def[halSDIO_Count] =
{
        {
            SDMMC1, RCC_APB2ENR_SDMMC1EN,
            {   GPIOC, GPIO_PIN_12, RCC_AHB1ENR_GPIOCEN, GPIO_AF12_SDMMC1  },
            {   GPIOD, GPIO_PIN_2,  RCC_AHB1ENR_GPIODEN, GPIO_AF12_SDMMC1  },
            {
                    {   GPIOC, GPIO_PIN_8,  RCC_AHB1ENR_GPIOCEN, GPIO_AF12_SDMMC1  },
                    {   GPIOC, GPIO_PIN_9,  RCC_AHB1ENR_GPIOCEN, GPIO_AF12_SDMMC1  },
                    {   GPIOC, GPIO_PIN_10, RCC_AHB1ENR_GPIOCEN, GPIO_AF12_SDMMC1  },
                    {   GPIOC, GPIO_PIN_11, RCC_AHB1ENR_GPIOCEN, GPIO_AF12_SDMMC1  }
            },
            {   GPIOI, GPIO_PIN_15, RCC_AHB1ENR_GPIOIEN }       // can be changed
        },
        {
            SDMMC2, RCC_APB2ENR_SDMMC2EN,
            {   GPIOD, GPIO_PIN_6,  RCC_AHB1ENR_GPIODEN, GPIO_AF11_SDMMC2  },
            {   GPIOD, GPIO_PIN_7,  RCC_AHB1ENR_GPIODEN, GPIO_AF11_SDMMC2  },
            {
                    {   GPIOG, GPIO_PIN_9,  RCC_AHB1ENR_GPIOGEN, GPIO_AF11_SDMMC2  },
                    {   GPIOG, GPIO_PIN_10, RCC_AHB1ENR_GPIOGEN, GPIO_AF11_SDMMC2  },
                    {   GPIOB, GPIO_PIN_3,  RCC_AHB1ENR_GPIOBEN, GPIO_AF10_SDMMC2  },
                    {   GPIOB, GPIO_PIN_4,  RCC_AHB1ENR_GPIOBEN, GPIO_AF10_SDMMC2  }
            },
            {   GPIOI, GPIO_PIN_15, RCC_AHB1ENR_GPIOIEN }       // can be changed
        }
};

static GPIO_InitTypeDef pinInitStruct = { .Speed = GPIO_SPEED_FREQ_MEDIUM };

static void halSDIOInitPin(const halSDIO_pinDef_t* pinData, halSDIO_PuPdType ppType,
        halSDIO_pinType pinType) {

    SET_BIT(RCC->AHB1ENR, pinData->port_rcc);

    pinInitStruct.Alternate = pinData->af;
    pinInitStruct.Mode = (pinType == halSDIO_pinSDIO) ? (GPIO_MODE_AF_PP) : (GPIO_MODE_INPUT);
    pinInitStruct.Pull = ppType;
    pinInitStruct.Pin = pinData->pin;

    HAL_GPIO_Init(pinData->port, &pinInitStruct);
}

static halSDIO_SDPresentState halSDIOSDIsDetected(halSDIO_struct* sdio_struct) {
    halSDIO_pinDef_t detectPin = sdio_def[sdio_struct->sdio].detectPin;
    return HAL_GPIO_ReadPin(detectPin.port, detectPin.pin) == GPIO_PIN_RESET ? halSDIO_Present : halSDIO_NotPresent;
}

halSDIO_error halSDIOInit(halSDIO_struct* sdio_struct, halSDIO_initStruct* initStruct) {
    if (sdio_struct == NULL || initStruct == NULL) {
    	return halSDIO_NULL_POINT;
    }

	SET_BIT(RCC->APB2ENR, sdio_def[sdio_struct->sdio].rcc);

    halSDIO_error SD_state = halSDIO_OK;

    SD_HandleTypeDef* sd_handle = (SD_HandleTypeDef*) sdio_struct;
    sd_handle->Instance = sdio_def[sdio_struct->sdio].instance;
    sd_handle->Init.ClockEdge = initStruct->clockEdge;
    sd_handle->Init.ClockBypass = initStruct->clockBypass;
    sd_handle->Init.ClockPowerSave = initStruct->powerSave;
    sd_handle->Init.BusWide = SDMMC_BUS_WIDE_1B;
    sd_handle->Init.HardwareFlowControl = initStruct->flowControl;
    sd_handle->Init.ClockDiv = initStruct->clockDiv;

    halSDIOInitPin(&sdio_def[sdio_struct->sdio].detectPin, halSDIO_Soft,
                halSDIO_pinDetect);

    if (halSDIOSDIsDetected(sdio_struct) != halSDIO_Present) {
        return halSDIO_SD_NOT_PRESENT;
    }
    halSDIOInitPin(&sdio_def[sdio_struct->sdio].cmd, initStruct->ppType,
            halSDIO_pinSDIO);
    halSDIOInitPin(&sdio_def[sdio_struct->sdio].ck, halSDIO_Hard,
            halSDIO_pinSDIO);

    for (uint8_t i = 0; i < (initStruct->wideWigth == halSDIO_1Bit ? WIDE_WIDTH_1 : WIDE_WIDTH_4);
            i++) {
        halSDIOInitPin(&sdio_def[sdio_struct->sdio].d[i], initStruct->ppType, halSDIO_pinSDIO);
    }

    if (HAL_SD_Init(sd_handle) != HAL_OK) {
        SD_state = halSDIO_OK;
    }

    if (SD_state == halSDIO_OK) {
        SD_state = HAL_SD_ConfigWideBusOperation(sd_handle, initStruct->wideWigth) != HAL_OK ? halSDIO_ERROR : halSDIO_OK;
    }

    return SD_state;
}

halSDIO_error halSDIOGetStatus(halSDIO_struct* sdio_struct)
{
    return sdio_struct == NULL ? halSDIO_NULL_POINT
    		: HAL_SD_GetCardStatus(&sdio_struct->sdHandle, &sdio_struct->sdStatus);
}

halSDIO_state halSDIOGetCardState(halSDIO_struct* sdio_struct)
{
    if (sdio_struct == NULL) {
    	return halSDIO_NULL_POINT;
    }
	return ((HAL_SD_GetCardState(&sdio_struct->sdHandle) == HAL_SD_CARD_TRANSFER) ?
            halSDIO_TRANSFER_OK : halSDIO_TRANSFER_BUSY);
}

void halSDIOGetCardInfo(halSDIO_struct* sdio_struct)
{
	if (sdio_struct == NULL) {
		return;
	}
	HAL_SD_GetCardInfo(&sdio_struct->sdHandle, &sdio_struct->sdInfo);
}

halSDIO_error halSDIOWriteBlocks(halSDIO_struct* sdio_struct, uint8_t *pData, uint32_t blockAdd, uint32_t blocksNumber, uint32_t timeout) {
		return sdio_struct == NULL || pData == NULL ? halSDIO_NULL_POINT
				: HAL_SD_WriteBlocks(&sdio_struct->sdHandle, pData, blockAdd, blocksNumber, timeout);
}

halSDIO_error halSDIOReadBlocks(halSDIO_struct* sdio_struct, uint8_t *pData, uint32_t blockAdd, uint32_t blocksNumber, uint32_t timeout) {
	return sdio_struct == NULL || pData == NULL ? halSDIO_NULL_POINT
			: HAL_SD_ReadBlocks(&sdio_struct->sdHandle, pData, blockAdd, blocksNumber, timeout);
}




