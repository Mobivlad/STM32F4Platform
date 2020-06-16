/*
 * hal_uart.c
 *
 *  Created on: 15 черв. 2020 р.
 *      Author: vladyslav.daliavskyi
 */

#include "hal_uart.h"

static halUARTErrorCode halUARTStatus[halUART_Count] = { halUART_NOT_CONFIG,
        halUART_NOT_CONFIG, halUART_NOT_CONFIG, halUART_NOT_CONFIG,
        halUART_NOT_CONFIG, halUART_NOT_CONFIG };

/// Convert function from halUART enumeration to USART_TypeDef
static USART_TypeDef* halUARTGetByEnum(halUART uart) {
    switch (uart) {
        case halUART1:
            return USART1;

        case halUART2:
            return USART2;

        case halUART3:
            return USART3;

        case halUART4:
            return UART4;

        case halUART5:
            return UART5;

        case halUART6:
            return USART6;

        default:
            return 0;
    }
}

/// Configure UART pins
static void halUARTInitGPIO(halUART uart) {
    GPIO_InitTypeDef gpio;
    gpio.GPIO_Speed = GPIO_Speed_50MHz;
    gpio.GPIO_OType = GPIO_OType_PP;
    gpio.GPIO_PuPd = GPIO_PuPd_UP;
    gpio.GPIO_Mode = GPIO_Mode_AF;
    switch (uart) {

        case halUART1:
            RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
            gpio.GPIO_Pin = GPIO_Pin_9;
            GPIO_Init(GPIOA, &gpio);
            GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_USART1);
            GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_USART1);
            return;

        case halUART2:
            RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
            gpio.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3;
            GPIO_Init(GPIOA, &gpio);
            GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_USART2);
            GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_USART2);
            return;

        case halUART3:
            RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
            gpio.GPIO_Pin = GPIO_Pin_11 | GPIO_Pin_10;
            GPIO_Init(GPIOB, &gpio);
            GPIO_PinAFConfig(GPIOB, GPIO_PinSource10, GPIO_AF_USART3);
            GPIO_PinAFConfig(GPIOB, GPIO_PinSource11, GPIO_AF_USART3);
            return;

        case halUART4:
            RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
            gpio.GPIO_Pin = GPIO_Pin_11 | GPIO_Pin_10;
            GPIO_Init(GPIOC, &gpio);
            GPIO_PinAFConfig(GPIOC, GPIO_PinSource10, GPIO_AF_UART4);
            GPIO_PinAFConfig(GPIOC, GPIO_PinSource11, GPIO_AF_UART4);
            return;

        case halUART5:
            RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
            gpio.GPIO_Pin = GPIO_Pin_12;
            GPIO_Init(GPIOC, &gpio);
            RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
            gpio.GPIO_Pin = GPIO_Pin_2;
            GPIO_Init(GPIOD, &gpio);
            GPIO_PinAFConfig(GPIOC, GPIO_PinSource12, GPIO_AF_UART5);
            GPIO_PinAFConfig(GPIOD, GPIO_PinSource2, GPIO_AF_UART5);
            return;

        case halUART6:
            RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
            gpio.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
            GPIO_Init(GPIOC, &gpio);
            GPIO_PinAFConfig(GPIOC, GPIO_PinSource6, GPIO_AF_USART6);
            GPIO_PinAFConfig(GPIOC, GPIO_PinSource7, GPIO_AF_USART6);
            return;

        default:
            return;
    }
}

/// Enable RCC function
static void halUARTEnableRCCByEnum(halUART uart) {
    switch (uart) {
        case halUART1:
            RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
            return;

        case halUART2:
            RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
            return;

        case halUART3:
            RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
            return;

        case halUART4:
            RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4, ENABLE);
            return;

        case halUART5:
            RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART5, ENABLE);
            return;

        case halUART6:
            RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART6, ENABLE);
            return;

        default:
            return;
    }
}

/// Enable interrupt function
static void halUARTEnableIRQByEnum(halUART uart) {
    switch (uart) {
        case halUART1:
            NVIC_EnableIRQ(USART1_IRQn);
            return;

        case halUART2:
            NVIC_EnableIRQ(USART2_IRQn);
            return;

        case halUART3:
            NVIC_EnableIRQ(USART3_IRQn);
            return;

        case halUART4:
            NVIC_EnableIRQ(UART4_IRQn);
            return;

        case halUART5:
            NVIC_EnableIRQ(UART5_IRQn);
            return;

        case halUART6:
            NVIC_EnableIRQ(USART6_IRQn);
            return;

        default:
            return;
    }
}

void halInitUART(halUART uart, halUARTInitStruct* initStruct) {
    // enable RCC
    halUARTEnableRCCByEnum(uart);
    // init pins for UART
    halUARTInitGPIO(uart);

    USART_InitTypeDef uartInitStruct;
    uartInitStruct.USART_BaudRate = initStruct->halUARTBaudRate;
    uartInitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    uartInitStruct.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    uartInitStruct.USART_Parity = initStruct->halUARTParity;
    uartInitStruct.USART_StopBits = initStruct->halUARTStopBits;
    uartInitStruct.USART_WordLength = initStruct->halUARTWordLength;
    USART_Init(halUARTGetByEnum(uart), &uartInitStruct);

    // enable interrupts
    halUARTEnableIRQByEnum(uart);

    // start UART communication
    USART_Cmd(halUARTGetByEnum(uart), ENABLE);

    halUARTStatus[uart] = halUART_OK;
}

static uint8_t inReceive[halUART_Count] = { 0 };
static uint8_t dataReceived[halUART_Count] = { 0 };
static uint8_t bufReceive[halUART_Count] = { 0 };

halUARTErrorCode halUARTReceiveByte(halUART uart, uint8_t* data) {
    // check on null point
    if (data == 0)
        return halUART_NULL_POINT;

    // check on configuration
    if (halUARTStatus[uart] == halUART_NOT_CONFIG)
        return halUART_NOT_CONFIG;

    // check receive status on busy
    if (!inReceive[uart]) {
        dataReceived[uart] = 0;
        inReceive[uart] = 1;

        // enable interrupts
        USART_ITConfig(halUARTGetByEnum(uart), USART_IT_RXNE, ENABLE);
    }

    // if dataReceived is 1
    if (dataReceived[uart]) {
        // return value
        *data = bufReceive[uart];
        // reset busy flag
        inReceive[uart] = 0;

        // return OK
        return halUART_OK;
    }
    return halUART_IN_PROGRESS;
}

static void ReceiveHandler(halUART uart) {
    // if interrupt on FLAG RXNE
    if (USART_GetITStatus(halUARTGetByEnum(uart), USART_IT_RXNE) == SET) {
        // turn off RXNE interrupts
        USART_ITConfig(halUARTGetByEnum(uart), USART_IT_RXNE, DISABLE);

        // Receive data byte
        bufReceive[uart] = USART_ReceiveData(halUARTGetByEnum(uart));
        // Set receive flag to 1
        dataReceived[uart] = 1;

        // Clear flag interrupt
        USART_ClearITPendingBit(halUARTGetByEnum(uart), USART_IT_RXNE);
    }
}

static uint8_t inTransfer[halUART_Count] = { 0 };
static uint8_t dataTransfered[halUART_Count] = { 0 };
static uint8_t bufTransfer[halUART_Count] = { 0 };

static void TransferHandler(halUART uart) {
    // if interrupt on FLAG TXE
    if (USART_GetITStatus(halUARTGetByEnum(uart), USART_IT_TXE) == SET) {
        // Turn off TXE interrupts
        USART_ITConfig(halUARTGetByEnum(uart), USART_IT_TXE, DISABLE);

        // Send data byte
        USART_SendData(halUARTGetByEnum(uart), bufTransfer[uart]);
        // Set transfer flag to 1
        dataTransfered[uart] = 1;

        // Clear flag interrupt
        USART_ClearITPendingBit(halUARTGetByEnum(uart), USART_IT_TXE);
    }
}

halUARTErrorCode halUARTSendByte(halUART uart, uint8_t data) {
    // check on configuration
    if (halUARTStatus[uart] == halUART_NOT_CONFIG)
        return halUART_NOT_CONFIG;

    // check transfer busy
    if (!inTransfer[uart]) {
        dataTransfered[uart] = 0;
        inTransfer[uart] = 1;
        bufTransfer[uart] = data;

        USART_ITConfig(halUARTGetByEnum(uart), USART_IT_TXE, ENABLE);
    }

    // check on transferring done
    if (dataTransfered[uart]) {
        // set transferring to ready status
        inTransfer[uart] = 0;

        // OK code
        return halUART_OK;
    }
    return halUART_IN_PROGRESS;
}

// Interrupt handler for USART1
void USART1_IRQHandler() {
    ReceiveHandler(halUART1);
    TransferHandler(halUART1);
}

// Interrupt handler for USART2
void USART2_IRQHandler() {
    ReceiveHandler(halUART2);
    TransferHandler(halUART2);
}

// Interrupt handler for USART3
void USART3_IRQHandler() {
    ReceiveHandler(halUART3);
    TransferHandler(halUART3);
}

// Interrupt handler for UART4
void UART4_IRQHandler() {
    ReceiveHandler(halUART4);
    TransferHandler(halUART4);
}

// Interrupt handler for UART5
void UART5_IRQHandler() {
    ReceiveHandler(halUART5);
    TransferHandler(halUART5);
}

// Interrupt handler for USART6
void USART6_IRQHandler() {
    ReceiveHandler(halUART6);
    TransferHandler(halUART6);
}

