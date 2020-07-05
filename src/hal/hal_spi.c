/*
 * hal_spi.c
 *
 *  Created on: 4 черв. 2020 р.
 *      Author: vladyslav.daliavskyi
 */
#include "hal_spi.h"

static const spi_data_struct spi_data[halSPI_Count] =
{
        {
                SPI1,
                { { GPIOA,    GPIO_Pin_7 },     GPIO_PinSource7,    RCC_AHB1Periph_GPIOA },
                { { GPIOA,    GPIO_Pin_6 },     GPIO_PinSource6,    RCC_AHB1Periph_GPIOA },
                { { GPIOA,    GPIO_Pin_5 },     GPIO_PinSource5,    RCC_AHB1Periph_GPIOA },
                { GPIOA,    GPIO_Pin_4 },
                GPIO_AF_SPI1, SPI1_IRQn, RCC_APB2Periph_SPI1, RCC_APB2PeriphClockCmd
        },
        {
                SPI2,
                { { GPIOB,    GPIO_Pin_15 },    GPIO_PinSource15,    RCC_AHB1Periph_GPIOB },
                { { GPIOB,    GPIO_Pin_14 },    GPIO_PinSource14,    RCC_AHB1Periph_GPIOB },
                { { GPIOB,    GPIO_Pin_13 },    GPIO_PinSource13,    RCC_AHB1Periph_GPIOB },
                { GPIOB,    GPIO_Pin_12 },
                GPIO_AF_SPI2, SPI1_IRQn, RCC_APB1Periph_SPI2, RCC_APB1PeriphClockCmd
        },
        {
                SPI3,
                { { GPIOB,    GPIO_Pin_5 },     GPIO_PinSource5,    RCC_AHB1Periph_GPIOB },
                { { GPIOB,    GPIO_Pin_4 },     GPIO_PinSource4,    RCC_AHB1Periph_GPIOB },
                { { GPIOB,    GPIO_Pin_3 },     GPIO_PinSource3,    RCC_AHB1Periph_GPIOB },
                { GPIOA,    GPIO_Pin_15 },
                GPIO_AF_SPI3, SPI1_IRQn, RCC_APB1Periph_SPI3, RCC_APB1PeriphClockCmd
        }
};

static halSPI_struct* halSPI_structsPointers[halSPI_Count] = { NULL };

halSPIErrorCode halSPIInit(halSPI_struct* spiStruct, halSPIInitStruct* initStruct) {
    if(spiStruct == NULL || initStruct == NULL) {
        return halSPI_DATA_NULL_POINTER;
    }

    spi_data[spiStruct->spi].spi_rcc_function(spi_data[spiStruct->spi].spi_rcc, ENABLE);

    RCC_AHB1PeriphClockCmd(spi_data[spiStruct->spi].miso.pin_rcc, ENABLE);
    RCC_AHB1PeriphClockCmd(spi_data[spiStruct->spi].mosi.pin_rcc, ENABLE);
    RCC_AHB1PeriphClockCmd(spi_data[spiStruct->spi].cls.pin_rcc, ENABLE);

    GPIO_InitTypeDef initGPIO;
    initGPIO.GPIO_PuPd = GPIO_PuPd_DOWN;
    initGPIO.GPIO_OType = GPIO_OType_PP;
    initGPIO.GPIO_Mode = GPIO_Mode_AF;
    initGPIO.GPIO_Speed = GPIO_Speed_50MHz;

    initGPIO.GPIO_Pin = spi_data[spiStruct->spi].cls.pin_data.pin;
    GPIO_Init(spi_data[spiStruct->spi].cls.pin_data.port, &initGPIO);
    initGPIO.GPIO_Pin = spi_data[spiStruct->spi].mosi.pin_data.pin;
    GPIO_Init(spi_data[spiStruct->spi].mosi.pin_data.port, &initGPIO);
    initGPIO.GPIO_Pin = spi_data[spiStruct->spi].miso.pin_data.pin;
    GPIO_Init(spi_data[spiStruct->spi].miso.pin_data.port, &initGPIO);

    GPIO_PinAFConfig(spi_data[spiStruct->spi].cls.pin_data.port,
            spi_data[spiStruct->spi].cls.pin_source, spi_data[spiStruct->spi].spi_af);
    GPIO_PinAFConfig(spi_data[spiStruct->spi].mosi.pin_data.port,
            spi_data[spiStruct->spi].mosi.pin_source, spi_data[spiStruct->spi].spi_af);
    GPIO_PinAFConfig(spi_data[spiStruct->spi].miso.pin_data.port,
            spi_data[spiStruct->spi].miso.pin_source, spi_data[spiStruct->spi].spi_af);

    if (initStruct->halSPITypeNSS == halSPINSSTypeHard) {
        return halSPI_OK;
    }
    initGPIO.GPIO_Mode = GPIO_Mode_OUT;
    initGPIO.GPIO_Pin = spi_data[spiStruct->spi].cs.pin;
    GPIO_Init(spi_data[spiStruct->spi].cs.port, &initGPIO);
    GPIO_SetBits(spi_data[spiStruct->spi].cs.port, spi_data[spiStruct->spi].cs.pin);

    SPI_InitTypeDef* SPI_initStruct = (SPI_InitTypeDef*)spiStruct;
    SPI_initStruct->SPI_BaudRatePrescaler = initStruct->halSPIFrequencyPrescaller;
    SPI_initStruct->SPI_DataSize = initStruct->halSPIDataSize;
    SPI_initStruct->SPI_Direction = initStruct->halSPIDirection;
    SPI_initStruct->SPI_FirstBit = initStruct->halSPIFirstBit;
    SPI_initStruct->SPI_Mode = SPI_Mode_Master;
    SPI_initStruct->SPI_NSS = initStruct->halSPITypeNSS;
    SPI_initStruct->SPI_CRCPolynomial = 7;

    switch(initStruct->halSPIMode){
        case halSPIMode0:
            SPI_initStruct->SPI_CPOL = SPI_CPOL_Low;
            SPI_initStruct->SPI_CPHA = SPI_CPHA_1Edge;
            break;
        case halSPIMode1:
            SPI_initStruct->SPI_CPOL = SPI_CPOL_Low;
            SPI_initStruct->SPI_CPHA = SPI_CPHA_2Edge;
            break;
        case halSPIMode2:
            SPI_initStruct->SPI_CPOL = SPI_CPOL_High;
            SPI_initStruct->SPI_CPHA = SPI_CPHA_1Edge;
            break;
        case halSPIMode3:
            SPI_initStruct->SPI_CPOL = SPI_CPOL_High;
            SPI_initStruct->SPI_CPHA = SPI_CPHA_2Edge;
            break;
    }

    SPI_Init(spi_data[spiStruct->spi].spi, SPI_initStruct);

    // disable all interrupts
    SPI_I2S_ITConfig(spi_data[spiStruct->spi].spi, SPI_I2S_IT_RXNE, DISABLE);
    SPI_I2S_ITConfig(spi_data[spiStruct->spi].spi, SPI_I2S_IT_TXE, DISABLE);

    // start SPI
    SPI_Cmd(spi_data[spiStruct->spi].spi, ENABLE);

    halSPI_structsPointers[spiStruct->spi] = spiStruct;

    // initial status update
    spiStruct->spiStatus |= halSPI_Config | halSPI_Ready;

    return halSPI_OK;
}

halSPIErrorCode halSPISetCS(halSPI_struct* spiStruct) {
    if (spiStruct == NULL) {
        return halSPI_DATA_NULL_POINTER;
    }
    if (!(spiStruct->spiStatus & halSPI_Config))
        return halSPI_NOT_CONFIG;

    GPIO_ResetBits(spi_data[spiStruct->spi].cs.port, spi_data[spiStruct->spi].cs.pin);
    return halSPI_OK;
}

halSPIErrorCode halSPIResetCS(halSPI_struct* spiStruct) {
    if (spiStruct == NULL) {
        return halSPI_DATA_NULL_POINTER;
    }
    if (!(spiStruct->spiStatus & halSPI_Config))
        return halSPI_NOT_CONFIG;

    GPIO_SetBits(spi_data[spiStruct->spi].cs.port, spi_data[spiStruct->spi].cs.pin);
    return halSPI_OK;
}

static halSPIErrorCode halSPISendReceive(halSPI_struct* spiStruct, halSPIOperation operation, uint8_t* data, uint16_t dataSize,
        halSPICallBack operationAction) {
    if (spiStruct == NULL) {
        return halSPI_DATA_NULL_POINTER;
    }

    // check on configuration
    if (!(spiStruct->spiStatus & halSPI_Config))
        return halSPI_NOT_CONFIG;

    // check on data null point
    if (data == NULL || dataSize == 0)
        return halSPI_DATA_NULL_POINTER;

    // check on SPI busy status
    if (!(spiStruct->spiStatus & halSPI_Ready))
        return halSPI_BUSY;

    // reset busy status
    spiStruct->spiStatus &= ~halSPI_Ready;

    // set write or read flag is status
    if(operation == halSPI_Read)
        spiStruct->spiStatus |= halSPI_ReadOperation;
    else
        spiStruct->spiStatus &= ~halSPI_ReadOperation;

    // set data operation parameters
    spiStruct->buffer = data;
    spiStruct->bufferSize = dataSize;
    spiStruct->rx_index = 0;
    spiStruct->tx_index = 0;
    spiStruct->callback = operationAction;

    // enable all interrupts
    SPI_I2S_ITConfig(spi_data[spiStruct->spi].spi, SPI_I2S_IT_RXNE, ENABLE);
    SPI_I2S_ITConfig(spi_data[spiStruct->spi].spi, SPI_I2S_IT_TXE, ENABLE);

    return halSPI_OK;
}

halSPIErrorCode halSPIReceive(halSPI_struct* spiStruct, uint8_t* data, uint16_t dataSize,
        halSPICallBack receiveAction) {
    return halSPISendReceive(spiStruct, halSPI_Read, data, dataSize, receiveAction);
}

halSPIErrorCode halSPISend(halSPI_struct* spiStruct, uint8_t* data, uint16_t dataSize,
        halSPICallBack sendAction) {
    return halSPISendReceive(spiStruct, halSPI_Write, data, dataSize, sendAction);
}

// general interrupt handler
static void spi_interrupt(halSPI spi){
    if (halSPI_structsPointers[spi] == NULL) {
        return;
    }

    // TX buffer is empty
    if (SPI_I2S_GetITStatus(spi_data[spi].spi, SPI_I2S_IT_TXE) != RESET){

        // if read operation
        if(halSPI_structsPointers[spi]->spiStatus & halSPI_ReadOperation){
            // send dummy byte
            SPI_I2S_SendData(spi_data[spi].spi, DUMMY_DATA);
            halSPI_structsPointers[spi]->tx_index++;
        } else {
            // send useful information
            SPI_I2S_SendData(spi_data[spi].spi,
                    halSPI_structsPointers[spi]->buffer[halSPI_structsPointers[spi]->tx_index++]);
        }
    }

    // RX buffer is not empty
    if (SPI_I2S_GetITStatus(spi_data[spi].spi, SPI_I2S_IT_RXNE) != RESET) {

        // if read operation
        if (halSPI_structsPointers[spi]->spiStatus & halSPI_ReadOperation) {
            // receive and save buffer data
            halSPI_structsPointers[spi]->buffer[halSPI_structsPointers[spi]->rx_index++] = SPI_I2S_ReceiveData(spi_data[spi].spi);
        } else {
            // simply receive data
            SPI_I2S_ReceiveData(spi_data[spi].spi);
            halSPI_structsPointers[spi]->rx_index++;
        }

        // if received necessary data count
        if (halSPI_structsPointers[spi]->rx_index == halSPI_structsPointers[spi]->bufferSize) {

            // disable add interrupts
            SPI_I2S_ITConfig(spi_data[spi].spi, SPI_I2S_IT_RXNE, DISABLE);
            SPI_I2S_ITConfig(spi_data[spi].spi, SPI_I2S_IT_TXE, DISABLE);

            // set status to ready
            halSPI_structsPointers[spi]->spiStatus |= halSPI_Ready;

            // call callback
            if(halSPI_structsPointers[spi]->callback != NULL)
                halSPI_structsPointers[spi]->callback();
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
