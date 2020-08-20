/*
 * hal_uart.c
 *
 *  Created on: 15 черв. 2020 р.
 *      Author: vladyslav.daliavskyi
 */

#include "hal_uart.h"

//+------+--------+--------+--------+--------+--------+--------+
//|      |  UART1 |  UART2 |  UART3 |  UART4 |  UART5 |  UART6 |
//+------+--------+--------+--------+--------+--------+--------+
//|  RX  |  PA10  |  PA3   |  PB11  |  PC11  |  PD2   |   PC7  |
//+------+--------+--------+--------+--------+--------+--------+
//|  TX  |  PA9   |  PA2   |  PB10  |  PC10  |  PC12  |   PC6  |
//+------+--------+--------+--------+--------+--------+--------+
// UART initial table
static const uart_data_struct usart_data[halUART_Count] =
{
        {
                USART1,
                {GPIOA,  GPIO_Pin_10,    GPIO_PinSource10,   RCC_AHB1Periph_GPIOA},
                {GPIOA,  GPIO_Pin_9,     GPIO_PinSource9,    RCC_AHB1Periph_GPIOA},
                GPIO_AF_USART1,         USART1_IRQn,
                RCC_APB2Periph_USART1,  RCC_APB2PeriphClockCmd
        },
        {
                USART2,
                {GPIOA,  GPIO_Pin_3,     GPIO_PinSource3,    RCC_AHB1Periph_GPIOA},
                {GPIOA,  GPIO_Pin_2,     GPIO_PinSource2,    RCC_AHB1Periph_GPIOA},
                GPIO_AF_USART2,         USART2_IRQn,
                RCC_APB1Periph_USART2,  RCC_APB1PeriphClockCmd
        },
        {
                USART3,
                {GPIOB,  GPIO_Pin_11,    GPIO_PinSource11,   RCC_AHB1Periph_GPIOB},
                {GPIOB,  GPIO_Pin_10,    GPIO_PinSource10,   RCC_AHB1Periph_GPIOB},
                GPIO_AF_USART3,         USART3_IRQn,
                RCC_APB1Periph_USART3,  RCC_APB1PeriphClockCmd
        },
        {
                UART4,
                {GPIOC,  GPIO_Pin_11,    GPIO_PinSource11,   RCC_AHB1Periph_GPIOC},
                {GPIOC,  GPIO_Pin_10,    GPIO_PinSource10,   RCC_AHB1Periph_GPIOC},
                GPIO_AF_UART4,          UART4_IRQn,
                RCC_APB1Periph_UART4,   RCC_APB1PeriphClockCmd
        },
        {
                UART5,
                {GPIOD,  GPIO_Pin_2,     GPIO_PinSource2,    RCC_AHB1Periph_GPIOD},
                {GPIOC,  GPIO_Pin_12,    GPIO_PinSource12,   RCC_AHB1Periph_GPIOC},
                GPIO_AF_UART5,          UART5_IRQn,
                RCC_APB1Periph_UART5,   RCC_APB1PeriphClockCmd
        },
        {
                USART6,
                {GPIOC,  GPIO_Pin_7,     GPIO_PinSource7,    RCC_AHB1Periph_GPIOC},
                {GPIOC,  GPIO_Pin_6,     GPIO_PinSource6,    RCC_AHB1Periph_GPIOC},
                GPIO_AF_USART6,         USART6_IRQn,
                RCC_APB2Periph_USART6,  RCC_APB2PeriphClockCmd
        }
};

static halUART_struct* halUART_structsPointers[halUART_Count] = { NULL };

halUARTErrorCode halUARTInit(halUART_struct* uartStruct, halUARTInitStruct* initStruct) {
    if (uartStruct == NULL || initStruct == NULL) {
        return halUART_NULL_POINT;
    }

    if (uartStruct->uartStatus & CONFIG) {
        return halUART_ALREADY_CONFIG;
    }
    // enable RCC
    usart_data[uartStruct->uart].usart_rcc_function(usart_data[uartStruct->uart].usart_rcc,
            ENABLE);

    // init pins for UART
    RCC_AHB1PeriphClockCmd(usart_data[uartStruct->uart].rx.pin_rcc, ENABLE);
    RCC_AHB1PeriphClockCmd(usart_data[uartStruct->uart].tx.pin_rcc, ENABLE);

    GPIO_InitTypeDef gpio;
    gpio.GPIO_Speed = GPIO_Speed_50MHz;
    gpio.GPIO_OType = GPIO_OType_PP;
    gpio.GPIO_PuPd  = GPIO_PuPd_NOPULL;
    gpio.GPIO_Mode  = GPIO_Mode_AF;

    gpio.GPIO_Pin   = usart_data[uartStruct->uart].rx.pin;
    GPIO_Init(usart_data[uartStruct->uart].rx.port, &gpio);
    GPIO_PinAFConfig(usart_data[uartStruct->uart].rx.port,
            usart_data[uartStruct->uart].rx.pin_source, usart_data[uartStruct->uart].usart_af);

    gpio.GPIO_Pin   = usart_data[uartStruct->uart].tx.pin;
    GPIO_Init(usart_data[uartStruct->uart].tx.port, &gpio);
    GPIO_PinAFConfig(usart_data[uartStruct->uart].tx.port,
            usart_data[uartStruct->uart].tx.pin_source, usart_data[uartStruct->uart].usart_af);

    USART_InitTypeDef* uartInitStruct = (USART_InitTypeDef*)uartStruct;
    uartInitStruct->USART_BaudRate               = initStruct->halUARTBaudRate;
    uartInitStruct->USART_HardwareFlowControl    = USART_HardwareFlowControl_None;
    uartInitStruct->USART_Mode                   = USART_Mode_Rx | USART_Mode_Tx;
    uartInitStruct->USART_Parity                 = initStruct->halUARTParity;
    uartInitStruct->USART_StopBits               = initStruct->halUARTStopBits;
    uartInitStruct->USART_WordLength             = initStruct->halUARTWordLength;

    USART_Init(usart_data[uartStruct->uart].usart, uartInitStruct);

    // enable interrupts
    NVIC_EnableIRQ(usart_data[uartStruct->uart].usart_irnq);

    // start UART communication
    USART_Cmd(usart_data[uartStruct->uart].usart, ENABLE);

    uartStruct->r_buffer = NULL;
    uartStruct->t_buffer = NULL;

    halUART_structsPointers[uartStruct->uart] = uartStruct;

    uartStruct->uartStatus = CONFIG | RX_READY | TX_READY;

    return halUART_OK;
}

halUARTErrorCode halUARTDeinit(halUART_struct* uartStruct) {
	if (uartStruct == NULL) {
		return halUART_NULL_POINT;
	}

	uartStruct->uartStatus = 0;

	USART_DeInit(usart_data[uartStruct->uart].usart);

	USART_Cmd(usart_data[uartStruct->uart].usart, DISABLE);

	uartStruct->r_buffer = NULL;
	uartStruct->t_buffer = NULL;

	usart_data[uartStruct->uart].usart_rcc_function(
			usart_data[uartStruct->uart].usart_rcc, DISABLE);

	NVIC_DisableIRQ(usart_data[uartStruct->uart].usart_irnq);

	halUART_structsPointers[uartStruct->uart] = NULL;

	return halUART_OK;
}

halUARTErrorCode halUARTReceive(halUART_struct* uartStruct, halUARTDataType dataType,
        uint8_t* buffer, uint16_t bufferSize, halUARTCallBack receiveCallBack) {
    if (uartStruct == NULL) {
        return halUART_NULL_POINT;
    }

    // check on configuration
    if (!(uartStruct->uartStatus & CONFIG)) {
        return halUART_NOT_CONFIG;
    }

    // check on RX busy
    if (!(uartStruct->uartStatus & RX_READY)) {
        return halUART_BUSY;
    }

    // check on data null point or zero size
    if (buffer == NULL || bufferSize == 0) {
        return halUART_NULL_POINT;
    }

    // set receive callback
    if (receiveCallBack != NULL) {
        uartStruct->rCallBack = receiveCallBack;
    } else {
        uartStruct->rCallBack = NULL;
    }

    // change RX busy flag
    uartStruct->uartStatus &= ~RX_READY;

    // set static parameters
    uartStruct->r_buffer = buffer;
    uartStruct->r_buffer_size = bufferSize;
    uartStruct->r_index = 0;

    // enable RXNE interrupts
    USART_ITConfig(usart_data[uartStruct->uart].usart, USART_IT_RXNE, ENABLE);

    // set unique parameters according to data type
    switch (dataType) {
        case halUARTDataType_Array:
            uartStruct->uartStatus &= ~RX_STRING;
            break;
        case halUARTDataType_String:
            uartStruct->uartStatus |= RX_STRING;
            USART_ClearITPendingBit(usart_data[uartStruct->uart].usart, USART_IT_IDLE);
            //USART_ITConfig(usart_data[uartStruct->uart].usart, USART_IT_IDLE, ENABLE);
            break;
    }

    return halUART_OK;
}

halUARTErrorCode halUARTTransfer(halUART_struct* uartStruct, halUARTDataType dataType,
        uint8_t* buffer, uint16_t bufferSize, halUARTCallBack transferCallBack) {
    if (uartStruct == NULL) {
        return halUART_NULL_POINT;
    }

    // check on configuration
    if (!(uartStruct->uartStatus & CONFIG)) {
        return halUART_NOT_CONFIG;
    }

    // check on TX busy
    if (!(uartStruct->uartStatus & TX_READY)) {
        return halUART_BUSY;
    }

    // check on buffer null point
    if (buffer == NULL) {
        return halUART_NULL_POINT;
    }

    // set transfer complete callback
    if (transferCallBack != NULL) {
        uartStruct->tCallBack = transferCallBack;
    } else {
        uartStruct->tCallBack = NULL;
    }

    // change TX flag on BYSY
    uartStruct->uartStatus &= ~TX_READY;

    // set transfer parameters
    uartStruct->t_buffer = buffer;
    uartStruct->t_buffer_size = bufferSize;
    uartStruct->t_index = 0;

    switch (dataType) {
        case halUARTDataType_Array:
            uartStruct->uartStatus &= ~TX_STRING;
            break;
        case halUARTDataType_String:
            uartStruct->uartStatus |= TX_STRING;
            break;
    }

    USART_ITConfig(usart_data[uartStruct->uart].usart, USART_IT_TXE, ENABLE);
    return halUART_OK;
}

static void uart_interupt(halUART uart) {
    if (USART_GetITStatus(usart_data[uart].usart, USART_IT_IDLE) != RESET) {
        if(halUART_structsPointers[uart]->r_index == 0){
            USART_ITConfig(usart_data[uart].usart, USART_IT_IDLE, DISABLE);
            return;
        }
        USART_ITConfig(usart_data[uart].usart, USART_IT_IDLE, DISABLE);
        USART_ClearITPendingBit(usart_data[uart].usart, USART_IT_IDLE);

        // change RX flag to ready
        halUART_structsPointers[uart]->uartStatus |= RX_READY;
        // check if received more then buffer size
        if (halUART_structsPointers[uart]->r_index > halUART_structsPointers[uart]->r_buffer_size) {
            if (halUART_structsPointers[uart]->overloadCallBack != NULL) {
                // call overload CallBack
                halUART_structsPointers[uart]->overloadCallBack();
            }
        } else if (halUART_structsPointers[uart]->rCallBack != NULL) {
            // end receiving callback
            halUART_structsPointers[uart]->rCallBack();
        }
    }

    if (USART_GetITStatus(usart_data[uart].usart, USART_IT_TXE) != RESET) {
        if (USART_GetFlagStatus(usart_data[uart].usart, USART_FLAG_TC) == SET) {
            if (halUART_structsPointers[uart]->uartStatus & TX_STRING) {
                if (halUART_structsPointers[uart]->t_buffer[halUART_structsPointers[uart]->t_index]
                        == '\0') {

                    USART_ITConfig(usart_data[uart].usart, USART_IT_TXE, DISABLE);

                    halUART_structsPointers[uart]->uartStatus |= TX_READY;

                    if (halUART_structsPointers[uart]->tCallBack != NULL) {
                        halUART_structsPointers[uart]->tCallBack();
                    }
                } else {
                    USART_SendData(usart_data[uart].usart,
                                            halUART_structsPointers[uart]->t_buffer[halUART_structsPointers[uart]->t_index++]);
                }
            } else {
                USART_SendData(usart_data[uart].usart,
                        halUART_structsPointers[uart]->t_buffer[halUART_structsPointers[uart]->t_index++]);

                if (halUART_structsPointers[uart]->t_index
                        == halUART_structsPointers[uart]->t_buffer_size) {

                    USART_ITConfig(usart_data[uart].usart, USART_IT_TXE, DISABLE);
                    halUART_structsPointers[uart]->uartStatus |= TX_READY;

                    if (halUART_structsPointers[uart]->tCallBack != NULL) {
                        halUART_structsPointers[uart]->tCallBack();
                    }
                }
            }
        }
    }
    if (USART_GetITStatus(usart_data[uart].usart, USART_IT_RXNE) != RESET) {
        if (halUART_structsPointers[uart]->uartStatus & RX_STRING) {
            USART_ITConfig(usart_data[uart].usart, USART_IT_IDLE, ENABLE);
            if (halUART_structsPointers[uart]->r_index
                    < halUART_structsPointers[uart]->r_buffer_size) {
                halUART_structsPointers[uart]->r_buffer[halUART_structsPointers[uart]->r_index++] =
                        USART_ReceiveData(usart_data[uart].usart);
            } else {
                USART_ReceiveData(usart_data[uart].usart);
                halUART_structsPointers[uart]->r_index++;
            }

        } else {
            halUART_structsPointers[uart]->r_buffer[halUART_structsPointers[uart]->r_index++] =
                    USART_ReceiveData(usart_data[uart].usart);
            if (halUART_structsPointers[uart]->r_index
                    == halUART_structsPointers[uart]->r_buffer_size) {

                USART_ITConfig(usart_data[uart].usart, USART_IT_RXNE, DISABLE);
                halUART_structsPointers[uart]->uartStatus |= RX_READY;

                if (halUART_structsPointers[uart]->rCallBack != NULL) {
                    halUART_structsPointers[uart]->rCallBack();
                }
            }
        }
    }
}

void halUARTSetOverloadCallBack(halUART_struct* uart_struct, halUARTCallBack callback){
    uart_struct->overloadCallBack = callback;
}

void halUARTClearOverloadCallBack(halUART_struct* uart_struct){
    uart_struct->overloadCallBack = NULL;
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

