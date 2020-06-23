/*
 * hal_spi.c
 *
 *  Created on: 4 черв. 2020 р.
 *      Author: vladyslav.daliavskyi
 */
#include "hal_spi.h"

/// array of SPI status flags
static uint8_t spiStatus[halSPICount] = { 0 };

// callback on action complete
static halSPICallBack callbacks[halSPICount];

// buffer for receive & transfer
static uint8_t* buffer[halSPICount];
static uint16_t bufferSize[halSPICount];

// transfer and receive iteration counter
static uint16_t tx_index[halSPICount];
static uint16_t rx_index[halSPICount];

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
        default:
            return;
    }
}

/**
 * Convert function with halSPI enumeration to SPI define type
 */
static SPI_TypeDef* halSPIGetByEnum(halSPI spi){
    switch(spi){
        case halSPI1:
            return SPI1;
        case halSPI2:
            return SPI2;
        case halSPI3:
            return SPI3;
        default:
            return 0;
    }
}

/**
 * GPIO pins init function for SPI
 */
static void halSPIGPIOInit(halSPI spi, halSPINSSType halSPITypeNSS){
    GPIO_InitTypeDef initGPIO;
    initGPIO.GPIO_PuPd = GPIO_PuPd_DOWN;
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
        default:
            break;
    }

    if(halSPITypeNSS == halSPINSSTypeHard) return;
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
        default:
            break;
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

    SPI_Init(halSPIGetByEnum(spi), &initStruct);

    // disable all interrupts
    SPI_I2S_ITConfig(halSPIGetByEnum(spi), SPI_I2S_IT_RXNE, DISABLE);
    SPI_I2S_ITConfig(halSPIGetByEnum(spi), SPI_I2S_IT_TXE, DISABLE);

    // start SPI
    SPI_Cmd(halSPIGetByEnum(spi), ENABLE);

    // initial status update
    spiStatus[spi] |= halSPI_Config | halSPI_Ready;
}

halSPIErrorCode halSPISetCS(halSPI spi) {
    if (!(spiStatus[spi] & halSPI_Config))
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
        default:
            return halSPI_NOT_CONFIG;
    }
}

halSPIErrorCode halSPIResetCS(halSPI spi) {
    if (!(spiStatus[spi] & halSPI_Config))
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
        default:
            return halSPI_NOT_CONFIG;
    }
}

static halSPIErrorCode halSPISendReceive(halSPI spi, halSPIOperation operation, uint8_t* data, uint16_t dataSize,
        halSPICallBack operationAction) {

    // check on configuration
    if (!(spiStatus[spi] & halSPI_Config))
        return halSPI_NOT_CONFIG;

    // check on data null point
    if (data == NULL || dataSize == 0)
        return halSPI_DATA_NULL_POINTER;

    // check on SPI busy status
    if (!(spiStatus[spi] & halSPI_Ready))
        return halSPI_BUSY;

    // reset busy status
    spiStatus[spi] &= ~halSPI_Ready;

    // set write or read flag is status
    if(operation == halSPI_Read)
        spiStatus[spi] |= halSPI_ReadOperation;
    else
        spiStatus[spi] &= ~halSPI_ReadOperation;

    // set data operation parameters
    buffer[spi] = data;
    bufferSize[spi] = dataSize;
    rx_index[spi] = 0;
    tx_index[spi] = 0;
    callbacks[spi] = operationAction;

    // enable all interrupts
    SPI_I2S_ITConfig(halSPIGetByEnum(spi), SPI_I2S_IT_RXNE, ENABLE);
    SPI_I2S_ITConfig(halSPIGetByEnum(spi), SPI_I2S_IT_TXE, ENABLE);

    return halSPI_OK;
}

halSPIErrorCode halSPIReceive(halSPI spi, uint8_t* data, uint16_t dataSize,
        halSPICallBack receiveAction) {
    return halSPISendReceive(spi, halSPI_Read, data, dataSize, receiveAction);
}

halSPIErrorCode halSPISend(halSPI spi, uint8_t* data, uint16_t dataSize,
        halSPICallBack sendAction) {
    return halSPISendReceive(spi, halSPI_Write, data, dataSize, sendAction);
}

// general interrupt handler
static void spi_interrupt(halSPI spi){
    // TX buffer is empty
    if (SPI_I2S_GetITStatus(halSPIGetByEnum(spi), SPI_I2S_IT_TXE) != RESET){

        // if read operation
        if(spiStatus[spi] & halSPI_ReadOperation){
            // send dummy byte
            SPI_I2S_SendData(halSPIGetByEnum(spi), DUMMY_DATA);
            tx_index[spi]++;
        } else {
            // send useful information
            SPI_I2S_SendData(halSPIGetByEnum(spi), buffer[spi][tx_index[spi]++]);
        }
    }

    // RX buffer is not empty
    if (SPI_I2S_GetITStatus(halSPIGetByEnum(spi), SPI_I2S_IT_RXNE) != RESET) {

        // if read operation
        if (spiStatus[spi] & halSPI_ReadOperation) {
            // receive and save buffer data
            buffer[spi][rx_index[spi]++] = SPI_I2S_ReceiveData(halSPIGetByEnum(spi));
        } else {
            // simply receive data
            SPI_I2S_ReceiveData(halSPIGetByEnum(spi));
            rx_index[spi]++;
        }

        // if received necessary data count
        if (rx_index[spi] == bufferSize[spi]) {

            // disable add interrupts
            SPI_I2S_ITConfig(halSPIGetByEnum(spi), SPI_I2S_IT_RXNE, DISABLE);
            SPI_I2S_ITConfig(halSPIGetByEnum(spi), SPI_I2S_IT_TXE, DISABLE);

            // set status to ready
            spiStatus[spi] |= halSPI_Ready;

            // call callback
            if(callbacks[spi] != NULL)
                callbacks[spi]();
        }
    }
}

// interrupt handler for SPI1
void SPI1_IRQHandler() {
    spi_interrupt(halSPI1);
}

// interrupt handler for SPI2
void SPI2_IRQHandler() {
    spi_interrupt(halSPI2);
}

// interrupt handler for SPI3
void SPI3_IRQHandler() {
    spi_interrupt(halSPI3);
}
