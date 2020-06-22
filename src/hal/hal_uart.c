/*
 * hal_uart.c
 *
 *  Created on: 15 черв. 2020 р.
 *      Author: vladyslav.daliavskyi
 */

#include "hal_uart.h"

static uint8_t uartStatus[halUART_Count] = { 0 };

static uint8_t* r_buffers[halUART_Count];
static uint16_t r_buffer_size[halUART_Count];
static halUARTCallBack rCallBacks[halUART_Count];
static uint16_t r_index[halUART_Count];

static uint8_t* t_buffers[halUART_Count];
static uint16_t t_buffer_size[halUART_Count];
static halUARTCallBack tCallBacks[halUART_Count];
static uint16_t t_index[halUART_Count];

static halUARTCallBack overloadCallBack[halUART_Count];

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
    gpio.GPIO_PuPd = GPIO_PuPd_NOPULL;
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

    r_buffers[uart] = NULL;
    t_buffers[uart] = NULL;

    uartStatus[uart] = CONFIG | RX_READY | TX_READY;
}

halUARTErrorCode halUARTReceive(halUART uart, halUARTDataType dataType, uint8_t* buffer,
        uint16_t bufferSize, halUARTCallBack receiveCallBack) {
    if (!(uartStatus[uart] & CONFIG)) {
        return halUART_NOT_CONFIG;
    }

    if (!(uartStatus[uart] & RX_READY)) {
        return halUART_BUSY;
    }

    if (buffer == NULL || bufferSize == 0) {
        return halUART_NULL_POINT;
    }

    if (receiveCallBack != NULL) {
        rCallBacks[uart] = receiveCallBack;
    } else {
        rCallBacks[uart] = NULL;
    }

    uartStatus[uart] &= ~RX_READY;
    r_buffers[uart] = buffer;
    r_buffer_size[uart] = bufferSize;
    r_index[uart] = 0;

    USART_ITConfig(halUARTGetByEnum(uart), USART_IT_RXNE, ENABLE);

    r_buffers[uart][r_index[uart]++] = USART_ReceiveData(halUARTGetByEnum(uart));

    switch (dataType) {
        case halUARTDataType_Array:
            uartStatus[uart] &= ~RX_STRING;
            break;
        case halUARTDataType_String:
            uartStatus[uart] |= RX_STRING;
            USART_ITConfig(halUARTGetByEnum(uart), USART_IT_IDLE, ENABLE);
            break;
    }

    return halUART_OK;
}

halUARTErrorCode halUARTTransfer(halUART uart, halUARTDataType dataType, uint8_t* buffer,
        uint16_t bufferSize, halUARTCallBack transferCallBack) {
    if (!(uartStatus[uart] & CONFIG)) {
        return halUART_NOT_CONFIG;
    }

    if (!(uartStatus[uart] & TX_READY)) {
        return halUART_BUSY;
    }

    if (buffer == NULL) {
        return halUART_NULL_POINT;
    }

    if (transferCallBack != NULL) {
        tCallBacks[uart] = transferCallBack;
    } else {
        tCallBacks[uart] = NULL;
    }

    uartStatus[uart] &= ~TX_READY;
    t_buffers[uart] = buffer;
    t_buffer_size[uart] = bufferSize;
    t_index[uart] = 0;

    switch (dataType) {
        case halUARTDataType_Array:
            uartStatus[uart] &= ~TX_STRING;
            break;
        case halUARTDataType_String:
            uartStatus[uart] |= TX_STRING;
            break;
    }

    USART_ITConfig(halUARTGetByEnum(uart), USART_IT_TXE, ENABLE);
    return halUART_OK;
}

static void uart_interupt(halUART uart) {
    if (USART_GetITStatus(halUARTGetByEnum(uart), USART_IT_IDLE) != RESET) {
        USART_ITConfig(halUARTGetByEnum(uart), USART_IT_IDLE, DISABLE);
        USART_ClearITPendingBit(halUARTGetByEnum(uart), USART_IT_IDLE);
        if (r_index[uart] > r_buffer_size[uart]) {
            if (overloadCallBack[uart] != NULL) {
                overloadCallBack[uart]();
            }
        } else if (rCallBacks[uart] != NULL) {
            rCallBacks[uart]();
        }
        uartStatus[uart] |= RX_READY;
    }

    if (USART_GetITStatus(halUARTGetByEnum(uart), USART_IT_TXE) != RESET) {
        if (uartStatus[uart] & TX_STRING) {
            USART_SendData(halUARTGetByEnum(uart), t_buffers[uart][t_index[uart]]);
            if (t_buffers[uart][t_index[uart]] == '\0') {
                if (tCallBacks[uart] != NULL) {
                    tCallBacks[uart]();
                }
                USART_ITConfig(halUARTGetByEnum(uart), USART_IT_TXE, DISABLE);
                uartStatus[uart] |= TX_READY;
            } else {
                t_index[uart]++;
            }
        } else {
            USART_SendData(halUARTGetByEnum(uart), t_buffers[uart][t_index[uart]++]);
            if (t_index[uart] == t_buffer_size[uart]) {
                if (tCallBacks[uart] != NULL) {
                    tCallBacks[uart]();
                }
                USART_ITConfig(halUARTGetByEnum(uart), USART_IT_TXE, DISABLE);
                uartStatus[uart] |= TX_READY;
            }
        }
    }

    if (USART_GetITStatus(halUARTGetByEnum(uart), USART_IT_RXNE) != RESET) {
        if (uartStatus[uart] & RX_STRING) {
            if (r_index[uart] < r_buffer_size[uart]) {
                r_buffers[uart][r_index[uart]++] = USART_ReceiveData(halUARTGetByEnum(uart));
            } else {
                USART_ReceiveData(halUARTGetByEnum(uart));
                r_index[uart]++;
            }
        } else {
            r_buffers[uart][r_index[uart]++] = USART_ReceiveData(halUARTGetByEnum(uart));
            if (r_index[uart] == r_buffer_size[uart]) {
                if (rCallBacks[uart] != NULL) {
                    rCallBacks[uart]();
                }
                USART_ITConfig(halUARTGetByEnum(uart), USART_IT_RXNE, DISABLE);
                uartStatus[uart] |= RX_READY;
            }
        }
    }
}

// Interrupt handler for USART1
void USART1_IRQHandler() {
    uart_interupt(halUART1);
}

// Interrupt handler for USART2
void USART2_IRQHandler() {
    uart_interupt(halUART2);
}

// Interrupt handler for USART3
void USART3_IRQHandler() {
    uart_interupt(halUART3);
}

// Interrupt handler for UART4
void UART4_IRQHandler() {
    uart_interupt(halUART4);
}

// Interrupt handler for UART5
void UART5_IRQHandler() {
    uart_interupt(halUART5);
}

// Interrupt handler for USART6
void USART6_IRQHandler() {
    uart_interupt(halUART6);
}

