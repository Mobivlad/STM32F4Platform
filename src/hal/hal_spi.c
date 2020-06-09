/*
 * hal_spi.c
 *
 *  Created on: 4 черв. 2020 р.
 *      Author: vladyslav.daliavskyi
 */
#include "hal_spi.h"

static void halSPIEnableRCCByEnam(halSPI spi){
    switch(spi){
        case halSPI1:
            RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1,ENABLE);
            return;
        case halSPI2:
            RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);
            return;
        case halSPI3:
            RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI3, ENABLE);
            return;
    }
}

static SPI_TypeDef* halSPIGetByEnam(halSPI spi){
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
    halSPIEnableRCCByEnam(spi);
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
    SPI_Init(halSPIGetByEnam(spi),&initStruct);
    SPI_Cmd(halSPIGetByEnam(spi), ENABLE);
}

void halSPISetCS(halSPI spi){
    switch (spi) {
        case halSPI1:
            GPIO_ResetBits(GPIOA, GPIO_Pin_4);
            return;
        case halSPI2:
            GPIO_ResetBits(GPIOB, GPIO_Pin_12);
            return;
        case halSPI3:
            GPIO_ResetBits(GPIOA, GPIO_Pin_15);
            return;
    }
}

void halSPIResetCS(halSPI spi){
    switch(spi){
        case halSPI1:
            GPIO_SetBits(GPIOA, GPIO_Pin_4);
            return;
        case halSPI2:
            GPIO_SetBits(GPIOB, GPIO_Pin_12);
            return;
        case halSPI3:
            GPIO_SetBits(GPIOA, GPIO_Pin_15);
            return;
    }
}

static halSPIErrorCode halSPISendReceiveByte(SPI_TypeDef* spi, uint16_t tData,
        uint16_t* rData, uint16_t timeout) {
    while (SPI_I2S_GetFlagStatus(spi, SPI_I2S_FLAG_BSY))
        if (--timeout)
            return halSPI_TIMEOUT;
    SPI_I2S_SendData(spi, tData);
    while (SPI_I2S_GetFlagStatus(spi, SPI_I2S_FLAG_BSY))
        if (--timeout)
            return halSPI_TIMEOUT;
    *rData = SPI_I2S_ReceiveData(spi);
    return halSPI_OK;
}

halSPIErrorCode halSPISendDataArray(halSPI halSPI, uint16_t* data,
        uint16_t dataLen, uint16_t timeout) {
    uint16_t* senddata = data;
    SPI_TypeDef* spi = halSPIGetByEnam(halSPI);
    uint16_t resData;
    for (uint16_t i = 0; i < dataLen; i++) {
        if (halSPISendReceiveByte(spi, *senddata, &resData, timeout)
                == halSPI_TIMEOUT)
            return halSPI_TIMEOUT;
        senddata++;
    }
    return halSPI_OK;
}

halSPIErrorCode halSPIReceiveDataArray(halSPI halSPI, uint16_t* data,
        uint16_t dataLen, uint16_t timeout) {
    uint16_t* resdata = data;
    SPI_TypeDef* spi = halSPIGetByEnam(halSPI);
    uint16_t sendData = 0x0000;
    for (uint16_t i = 0; i < dataLen; i++) {
        if (halSPISendReceiveByte(spi, sendData, resdata, timeout)
                == halSPI_TIMEOUT)
            return halSPI_TIMEOUT;
        resdata++;
    }
    return halSPI_OK;
}

