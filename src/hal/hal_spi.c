/*
 * hal_spi.c
 *
 *  Created on: 4 черв. 2020 р.
 *      Author: vladyslav.daliavskyi
 */
#include "hal_spi.h"

//+------+--------+--------+--------+--------+
//|      |  MOSI  |  MISO  |  CLS   |   CS   |
//+------+--------+--------+--------+--------+
//| SPI1 |  PB5   |  PB4   |  PB3   |  PA15  |
//+------+--------+--------+--------+--------+
//| SPI2 |  PI3   |  PI2   |  PA12  |  PB9   |
//+------+--------+--------+--------+--------+
//| SPI3 |  PC12  |  PC11  |  PC10  |  PA4   |
//+------+--------+--------+--------+--------+
//| SPI4 |  PE6   |  PE5   |  PE2   |  PE4   |
//+------+--------+--------+--------+--------+
//| SPI5 |  PF9   |  PF8   |  PF7   |  PH5   |
//+------+--------+--------+--------+--------+
//| SPI6 |  PG14  |  PG12  |  PG13  |  PG8   |
//+------+--------+--------+--------+--------+

static const spi_data_struct spi_data[halSPI_Count] =
{
        {
                SPI1,
                { GPIOB,    GPIO_PIN_5,     RCC_AHB1ENR_GPIOBEN, GPIO_AF5_SPI1 },
                { GPIOB,    GPIO_PIN_4,     RCC_AHB1ENR_GPIOBEN, GPIO_AF5_SPI1 },
                { GPIOB,    GPIO_PIN_3,     RCC_AHB1ENR_GPIOBEN, GPIO_AF5_SPI1 },
                { GPIOA,    GPIO_PIN_15,    RCC_AHB1ENR_GPIOBEN, GPIO_AF5_SPI1 },
                APB2, RCC_APB2ENR_SPI1EN
        },
        {
                SPI2,
                { GPIOI,    GPIO_PIN_3,     RCC_AHB1ENR_GPIOIEN, GPIO_AF5_SPI2 },
                { GPIOI,    GPIO_PIN_2,     RCC_AHB1ENR_GPIOIEN, GPIO_AF5_SPI2 },
                { GPIOA,    GPIO_PIN_12,    RCC_AHB1ENR_GPIOAEN, GPIO_AF5_SPI2 },
                { GPIOB,    GPIO_PIN_9,     RCC_AHB1ENR_GPIOBEN, GPIO_AF5_SPI2 },
                APB1, RCC_APB1ENR_SPI2EN
        },
        {
                SPI3,
                { GPIOC,    GPIO_PIN_12,    RCC_AHB1ENR_GPIOCEN, GPIO_AF6_SPI3 },
                { GPIOC,    GPIO_PIN_11,    RCC_AHB1ENR_GPIOCEN, GPIO_AF6_SPI3 },
                { GPIOC,    GPIO_PIN_10,    RCC_AHB1ENR_GPIOCEN, GPIO_AF6_SPI3 },
                { GPIOA,    GPIO_PIN_4,     RCC_AHB1ENR_GPIOAEN, GPIO_AF6_SPI3 },
                APB1, RCC_APB1ENR_SPI3EN
        },
        {
                SPI4,
                { GPIOE,    GPIO_PIN_6,     RCC_AHB1ENR_GPIOEEN, GPIO_AF5_SPI4 },
                { GPIOE,    GPIO_PIN_5,     RCC_AHB1ENR_GPIOEEN, GPIO_AF5_SPI4 },
                { GPIOE,    GPIO_PIN_2,     RCC_AHB1ENR_GPIOEEN, GPIO_AF5_SPI4 },
                { GPIOE,    GPIO_PIN_4,     RCC_AHB1ENR_GPIOEEN, GPIO_AF5_SPI4 },
                APB2, RCC_APB2ENR_SPI4EN
        },
        {
                SPI5,
                { GPIOF,    GPIO_PIN_9,     RCC_AHB1ENR_GPIOFEN, GPIO_AF5_SPI5 },
                { GPIOF,    GPIO_PIN_8,     RCC_AHB1ENR_GPIOFEN, GPIO_AF5_SPI5 },
                { GPIOF,    GPIO_PIN_7,     RCC_AHB1ENR_GPIOFEN, GPIO_AF5_SPI5 },
                { GPIOH,    GPIO_PIN_5,     RCC_AHB1ENR_GPIOHEN, GPIO_AF5_SPI5 },
                APB2, RCC_APB2ENR_SPI5EN
        },
        {
                SPI6,
                { GPIOG,    GPIO_PIN_14,    RCC_AHB1ENR_GPIOGEN, GPIO_AF5_SPI6 },
                { GPIOG,    GPIO_PIN_12,    RCC_AHB1ENR_GPIOGEN, GPIO_AF5_SPI6 },
                { GPIOG,    GPIO_PIN_13,    RCC_AHB1ENR_GPIOGEN, GPIO_AF5_SPI6 },
                { GPIOG,    GPIO_PIN_8,     RCC_AHB1ENR_GPIOGEN, GPIO_AF5_SPI6 },
                APB2, RCC_APB2ENR_SPI6EN
        }
};

static GPIO_InitTypeDef pinInitStruct = {
        .Speed = GPIO_SPEED_FREQ_MEDIUM,
        .Pull = GPIO_NOPULL
};

halSPI_errorCode halSPIInit(halSPI_struct* spiStruct, halSPI_initStruct* initialStruct) {
    if(spiStruct == NULL || initialStruct == NULL) {
        return halSPI_DATA_NULL_POINTER;
    }

    switch (spi_data[spiStruct->spi].bus) {
        case APB1:
            SET_BIT(RCC->APB1ENR, spi_data[spiStruct->spi].rcc);
            break;
        case APB2:
            SET_BIT(RCC->APB2ENR, spi_data[spiStruct->spi].rcc);
            break;
    }

    SET_BIT(RCC->AHB1ENR, spi_data[spiStruct->spi].mosi.port_rcc);
    SET_BIT(RCC->AHB1ENR, spi_data[spiStruct->spi].miso.port_rcc);
    SET_BIT(RCC->AHB1ENR, spi_data[spiStruct->spi].cls.port_rcc);

    pinInitStruct.Mode = GPIO_MODE_AF_PP;

    pinInitStruct.Alternate = spi_data[spiStruct->spi].mosi.af;
    pinInitStruct.Pin = spi_data[spiStruct->spi].mosi.pin;
    HAL_GPIO_Init(spi_data[spiStruct->spi].mosi.port, &pinInitStruct);

    pinInitStruct.Alternate = spi_data[spiStruct->spi].miso.af;
    pinInitStruct.Pin = spi_data[spiStruct->spi].miso.pin;
    HAL_GPIO_Init(spi_data[spiStruct->spi].miso.port, &pinInitStruct);

    pinInitStruct.Alternate = spi_data[spiStruct->spi].cls.af;
    pinInitStruct.Pin = spi_data[spiStruct->spi].cls.pin;
    HAL_GPIO_Init(spi_data[spiStruct->spi].cls.port, &pinInitStruct);

    if (initialStruct->typeNSS == halSPI_NSSTypeSoft) {
        SET_BIT(RCC->AHB1ENR, spi_data[spiStruct->spi].cls.port_rcc);
        pinInitStruct.Alternate = spi_data[spiStruct->spi].cs.af;
        pinInitStruct.Pin = spi_data[spiStruct->spi].cs.pin;
        pinInitStruct.Mode = GPIO_MODE_OUTPUT_PP;
        HAL_GPIO_Init(spi_data[spiStruct->spi].cls.port, &pinInitStruct);

        HAL_GPIO_WritePin(spi_data[spiStruct->spi].cs.port, spi_data[spiStruct->spi].cs.pin, GPIO_PIN_SET);
    }

    SPI_HandleTypeDef* initStruct = (SPI_HandleTypeDef*)spiStruct;

    initStruct->Instance = spi_data[spiStruct->spi].spi;
    initStruct->Init.Mode = SPI_MODE_MASTER;
    initStruct->Init.Direction = initialStruct->direction;
    initStruct->Init.DataSize = initialStruct->dataSize;

    switch (initialStruct->mode) {
        case halSPI_Mode0:
            initStruct->Init.CLKPolarity = SPI_POLARITY_LOW;
            initStruct->Init.CLKPhase = SPI_PHASE_1EDGE;
            break;
        case halSPI_Mode1:
            initStruct->Init.CLKPolarity = SPI_POLARITY_LOW;
            initStruct->Init.CLKPhase = SPI_PHASE_2EDGE;
            break;
        case halSPI_Mode2:
            initStruct->Init.CLKPolarity = SPI_POLARITY_HIGH;
            initStruct->Init.CLKPhase = SPI_PHASE_1EDGE;
            break;
        case halSPI_Mode3:
            initStruct->Init.CLKPolarity = SPI_POLARITY_HIGH;
            initStruct->Init.CLKPhase = SPI_PHASE_2EDGE;
            break;
    }

    initStruct->Init.NSS = initialStruct->typeNSS;
    initStruct->Init.BaudRatePrescaler = initialStruct->frequencyPrescaller;
    initStruct->Init.FirstBit = initialStruct->firstBit;

    initStruct->Init.TIMode = SPI_TIMODE_DISABLE;
    initStruct->Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
    initStruct->Init.CRCPolynomial = 7;
    initStruct->Init.CRCLength = SPI_CRC_LENGTH_DATASIZE;
    initStruct->Init.NSSPMode = SPI_NSS_PULSE_ENABLE;
    HAL_SPI_Init(initStruct);

    // initial status update
    spiStruct->spiStatus |= halSPI_Config | halSPI_Ready;

    return halSPI_OK;
}

halSPI_errorCode halSPISetCS(halSPI_struct* spiStruct) {
    if (spiStruct == NULL) {
        return halSPI_DATA_NULL_POINTER;
    }
    if (!(spiStruct->spiStatus & halSPI_Config))
        return halSPI_NOT_CONFIG;

    HAL_GPIO_WritePin(spi_data[spiStruct->spi].cs.port, spi_data[spiStruct->spi].cs.pin, GPIO_PIN_RESET);
    return halSPI_OK;
}

halSPI_errorCode halSPIResetCS(halSPI_struct* spiStruct) {
    if (spiStruct == NULL) {
        return halSPI_DATA_NULL_POINTER;
    }
    if (!(spiStruct->spiStatus & halSPI_Config))
        return halSPI_NOT_CONFIG;

    HAL_GPIO_WritePin(spi_data[spiStruct->spi].cs.port, spi_data[spiStruct->spi].cs.pin, GPIO_PIN_SET);
    return halSPI_OK;
}

halSPI_errorCode halSPIIsReady(halSPI_struct* spiStruct) {
    return (spiStruct->spiStatus & halSPI_Ready) ? halSPI_OK : halSPI_BUSY;
}

halSPI_errorCode halSPISend(halSPI_struct* spiStruct, uint8_t* data, uint16_t dataSize,
        uint16_t timeout) {
    return HAL_SPI_Transmit(&spiStruct->spi_def, data, dataSize, timeout);
}

halSPI_errorCode halSPIReceive(halSPI_struct* spiStruct, uint8_t* data, uint16_t dataSize,
        uint16_t timeout) {
    return HAL_SPI_Receive(&spiStruct->spi_def, data, dataSize, timeout);
}
