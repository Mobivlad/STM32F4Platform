/*
 * drv_com_port.c
 *
 *  Created on: Jul 10, 2020
 *      Author: Sigma
 */

#include "drv_com_port.h"

static const com_port_struct com_port_data[drvCOMPort_Count] =
{
        {

                .uart       = halUART4,
                .baudrate   = halUARTBaudRate9600,
                .wordlen    = USART_WordLength_8b,
                .stopbits   = halUARTStopBits1,
                .parity     = USART_Parity_No
        }
};

drvCOMPortErrorCode  drvCOMPortInit(drvCOMPort_struct* comPortStruct,  drvCOMPort comPort) {
    halUARTInitStruct initStruct;
    initStruct.halUARTBaudRate = com_port_data[comPort].baudrate;
    initStruct.halUARTParity = com_port_data[comPort].parity;
    initStruct.halUARTStopBits = com_port_data[comPort].stopbits;
    initStruct.halUARTWordLength = com_port_data[comPort].wordlen;

    ((halUART_struct*)comPortStruct)->uart = com_port_data[comPort].uart;

    return halUARTInit((halUART_struct*)comPortStruct, &initStruct);
}

drvCOMPortErrorCode  drvCOMPortDeinit(drvCOMPort_struct* comPortStruct) {
    return halUARTDeinit((halUART_struct*)comPortStruct);
}

drvCOMPortErrorCode drvCOMPortReadString(drvCOMPort_struct* comPortStruct, char* stringDest, uint16_t destSize, drvCOMPortCallBack completeAction) {
    return halUARTReceive((halUART_struct*)comPortStruct, halUARTDataType_String, (uint8_t*)stringDest, destSize, completeAction);
}

drvCOMPortErrorCode  drvCOMPortReadArray(drvCOMPort_struct* comPortStruct, uint8_t* arrayDest, uint16_t arraySize, drvCOMPortCallBack completeAction) {
    return halUARTReceive((halUART_struct*)comPortStruct, halUARTDataType_Array, arrayDest, arraySize, completeAction);
}

drvCOMPortErrorCode  drvCOMPortWriteString(drvCOMPort_struct* comPortStruct, char* stringSrc, drvCOMPortCallBack completeAction) {
    return halUARTTransfer((halUART_struct*)comPortStruct, halUARTDataType_String, (uint8_t*)stringSrc, strlen(stringSrc), completeAction);
}

drvCOMPortErrorCode  drvCOMPortWriteArray(drvCOMPort_struct* comPortStruct, uint8_t* arraySrc, uint16_t arraySize, drvCOMPortCallBack completeAction) {
    return halUARTTransfer((halUART_struct*)comPortStruct, halUARTDataType_Array, (uint8_t*)arraySrc, arraySize, completeAction);
}
