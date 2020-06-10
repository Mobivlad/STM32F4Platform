/*
 * hal_spi.c
 *
 *  Created on: 4 черв. 2020 р.
 *      Author: vladyslav.daliavskyi
 */
#include "hal_spi.h"

static halSPIStatus spiStatus[halSPICount] = { halSPINotConfigured,
        halSPINotConfigured, halSPINotConfigured };

static void halSPIEnableRCCByEnum(halSPI spi){
    switch(spi){
        case halSPI1:
            RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);
            return;
        case halSPI2:
            RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);
            return;
        case halSPI3:
            RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI3, ENABLE);
            return;
    }
}

static SPI_TypeDef* halSPIGetByEnum(halSPI spi){
    switch(spi){
        case halSPI1:
            return SPI1;
        case halSPI2:
            return SPI2;
        case halSPI3:
            return SPI3;
    }
    return 0;
}

static void halSPIGPIOInit(halSPI spi,halSPINSSType halSPITypeNSS){
    GPIO_InitTypeDef initGPIO;
    initGPIO.GPIO_PuPd = GPIO_PuPd_NOPULL;
    initGPIO.GPIO_OType = GPIO_OType_PP;
    initGPIO.GPIO_Mode = GPIO_Mode_AF;
    initGPIO.GPIO_Speed = GPIO_Speed_50MHz;
    switch(spi){
        case halSPI1:{
            RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
            initGPIO.GPIO_Pin = GPIO_Pin_5| GPIO_Pin_6| GPIO_Pin_7;
            GPIO_Init(GPIOA, &initGPIO);
            GPIO_PinAFConfig(GPIOA, GPIO_PinSource5, GPIO_AF_SPI1);
            GPIO_PinAFConfig(GPIOA, GPIO_PinSource6, GPIO_AF_SPI1);
            GPIO_PinAFConfig(GPIOA, GPIO_PinSource7, GPIO_AF_SPI1);
            break;
        }
        case halSPI2:{
            RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
            initGPIO.GPIO_Pin = GPIO_Pin_15| GPIO_Pin_13| GPIO_Pin_14;
            GPIO_Init(GPIOB, &initGPIO);
            GPIO_PinAFConfig(GPIOB, GPIO_PinSource15, GPIO_AF_SPI2);
            GPIO_PinAFConfig(GPIOB, GPIO_PinSource13, GPIO_AF_SPI2);
            GPIO_PinAFConfig(GPIOB, GPIO_PinSource14, GPIO_AF_SPI2);
            break;
        }
        case halSPI3:{
            RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
            initGPIO.GPIO_Pin = GPIO_Pin_3| GPIO_Pin_4| GPIO_Pin_5;
            GPIO_Init(GPIOB, &initGPIO);
            GPIO_PinAFConfig(GPIOB, GPIO_PinSource3, GPIO_AF_SPI3);
            GPIO_PinAFConfig(GPIOB, GPIO_PinSource4, GPIO_AF_SPI3);
            GPIO_PinAFConfig(GPIOB, GPIO_PinSource5, GPIO_AF_SPI3);
            break;
        }
    }

    if(halSPITypeNSS==halSPINSSTypeHard)return;
    initGPIO.GPIO_Mode = GPIO_Mode_OUT;
    switch (spi) {
        case halSPI1: {
            initGPIO.GPIO_Pin = GPIO_Pin_4;
            GPIO_Init(GPIOA, &initGPIO);
            GPIO_SetBits(GPIOA, GPIO_Pin_4);
            break;
        }
        case halSPI2: {
            RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
            initGPIO.GPIO_Pin = GPIO_Pin_12;
            GPIO_Init(GPIOB, &initGPIO);
            GPIO_SetBits(GPIOB, GPIO_Pin_12);
            break;
        }
        case halSPI3: {
            initGPIO.GPIO_Pin = GPIO_Pin_15;
            GPIO_Init(GPIOA, &initGPIO);
            GPIO_SetBits(GPIOA, GPIO_Pin_15);
            break;
        }
    }
}

void halSPIInit(halSPI spi, halSPIInitStruct* spiInitStruct){
    halSPIEnableRCCByEnum(spi);
    halSPIGPIOInit(spi,spiInitStruct->halSPITypeNSS);

    SPI_InitTypeDef initStruct;
    initStruct.SPI_BaudRatePrescaler = spiInitStruct->halSPIFrequencyPrescaller;
    initStruct.SPI_DataSize = spiInitStruct->halSPIDataSize;
    initStruct.SPI_Direction = spiInitStruct->halSPIDirection;
    initStruct.SPI_FirstBit = spiInitStruct->halSPIFirstBit;
    initStruct.SPI_Mode = SPI_Mode_Master;
    initStruct.SPI_NSS = spiInitStruct->halSPITypeNSS;
    initStruct.SPI_CRCPolynomial = 7;

    switch(spiInitStruct->halSPIMode){
        case halSPIMode0:
            initStruct.SPI_CPOL = SPI_CPOL_Low;
            initStruct.SPI_CPHA = SPI_CPHA_1Edge;
            break;
        case halSPIMode1:
            initStruct.SPI_CPOL = SPI_CPOL_Low;
            initStruct.SPI_CPHA = SPI_CPHA_2Edge;
            break;
        case halSPIMode2:
            initStruct.SPI_CPOL = SPI_CPOL_High;
            initStruct.SPI_CPHA = SPI_CPHA_1Edge;
            break;
        case halSPIMode3:
            initStruct.SPI_CPOL = SPI_CPOL_High;
            initStruct.SPI_CPHA = SPI_CPHA_2Edge;
            break;
    }

    SPI_Init(halSPIGetByEnum(spi),&initStruct);
    SPI_Cmd(halSPIGetByEnum(spi), ENABLE);

    //  Configuration interruptions
    SPI_I2S_ITConfig(halSPIGetByEnum(spi), SPI_I2S_IT_TXE, ENABLE);
    SPI_I2S_ITConfig(halSPIGetByEnum(spi), SPI_I2S_IT_RXNE, ENABLE);

    // Clear interruptions' flags
    SPI_I2S_ClearITPendingBit(SPI1, SPI_I2S_IT_TXE);
    SPI_I2S_ClearITPendingBit(SPI1, SPI_I2S_IT_RXNE);

    spiStatus[spi]=halSPIReadyToTransmit;
}

halSPIErrorCode halSPISetCS(halSPI spi) {
    if (spiStatus[spi] == halSPINotConfigured)
        return halSPI_NOT_CONFIG;

    switch (spi) {
        case halSPI1:
            GPIO_ResetBits(GPIOA, GPIO_Pin_4);
            return halSPI_OK;
        case halSPI2:
            GPIO_ResetBits(GPIOB, GPIO_Pin_12);
            return halSPI_OK;
        case halSPI3:
            GPIO_ResetBits(GPIOA, GPIO_Pin_15);
            return halSPI_OK;
    }
    return halSPI_NOT_CONFIG;
}

halSPIErrorCode halSPIResetCS(halSPI spi) {
    if (spiStatus[spi] == halSPINotConfigured)
        return halSPI_NOT_CONFIG;

    switch (spi) {
        case halSPI1:
            GPIO_SetBits(GPIOA, GPIO_Pin_4);
            return halSPI_OK;
        case halSPI2:
            GPIO_SetBits(GPIOB, GPIO_Pin_12);
            return halSPI_OK;
        case halSPI3:
            GPIO_SetBits(GPIOA, GPIO_Pin_15);
            return halSPI_OK;
    }
    return halSPI_NOT_CONFIG;
}

halSPIErrorCode halSPISendByte(halSPI spi, uint16_t* src) {
    if (spiStatus[spi] == halSPI_NOT_CONFIG)
        return halSPI_NOT_CONFIG;

    if (src == 0)
        return halSPI_DATA_NULL_POINTER;

    if (spiStatus[spi] == halSPIReadyToTransmit)
        SPI_I2S_SendData(halSPIGetByEnum(spi), *src);

    if (spiStatus[spi] == halSPIReadyToReceive) {
        SPI_I2S_ReceiveData(halSPIGetByEnum(spi));
        return halSPI_OK;
    }
    return halSPI_IN_PROGRESS;
}

halSPIErrorCode halSPIReceiveByte(halSPI spi, uint16_t* dest) {
    if (spiStatus[spi] == halSPI_NOT_CONFIG)
        return halSPI_NOT_CONFIG;

    if (dest == 0)
        return halSPI_DATA_NULL_POINTER;

    if (spiStatus[spi] == halSPIReadyToTransmit)
        SPI_I2S_SendData(halSPIGetByEnum(spi), DUMMY_DATA);

    if (spiStatus[spi] == halSPIReadyToReceive) {
        *dest = SPI_I2S_ReceiveData(halSPIGetByEnum(spi));
        return halSPI_OK;
    }
    return halSPI_IN_PROGRESS;
}

static void changeStatus(halSPI spi) {
    if (SPI_I2S_GetITStatus(SPI1, SPI_I2S_IT_RXNE) == SET) {
        spiStatus[spi] = halSPIReadyToReceive;
        SPI_I2S_ClearITPendingBit(halSPI1, SPI_I2S_IT_RXNE);
        return;
    }
    if (SPI_I2S_GetITStatus(SPI1, SPI_I2S_IT_TXE) == SET) {
        spiStatus[spi] = halSPIReadyToTransmit;
        SPI_I2S_ClearITPendingBit(halSPI1, SPI_I2S_IT_TXE);
        return;
    }
}

void SPI1_IRQHandler() {
    changeStatus(halSPI1);
}

void SPI2_IRQHandler() {
    changeStatus(halSPI2);
}

void SPI3_IRQHandler() {
    changeStatus(halSPI3);
}

