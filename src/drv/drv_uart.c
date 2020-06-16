/*
 * drv_uart.c
 *
 *  Created on: 16 черв. 2020 р.
 *      Author: vladyslav.daliavskyi
 */

#include "drv_uart.h"
// counters for success iterations
static uint16_t sendIteration[drvUART_Count];
static uint16_t receiveIteration[drvUART_Count];

void drvInitUART(drvUART uart, drvUARTInitStruct* initStruct){
    halInitUART(uart, (halUARTInitStruct*)initStruct);

    sendIteration[drvUART_Count] = 0;
    receiveIteration[drvUART_Count] = 0;
}

drvUARTErrorCode drvSendDataArray(drvUART uart, uint8_t* data, uint16_t size) {
    // checking on null
    if (data == 0)
        return drvUART_NULL_POINT;

    // if transfered necessary count of byte
    if (sendIteration[uart] == size) {
        // clear iteration counter
        sendIteration[uart] = 0;

        // return OK code
        return drvUART_OK;
    }

    // Try to transfer byte
    drvUARTErrorCode errCode = halUARTSendByte(uart, data[sendIteration[uart]]);

    switch(errCode){
        // if transfer code is OK
        case drvUART_OK:
            // next iteration
            sendIteration[uart]++;
            break;
        default:
            // if error
            return errCode;
    }

    return drvUART_IN_PROGRESS;
}

drvUARTErrorCode drvSendString(drvUART uart, char* str) {
    // checking on null
    if (str == 0)
        return drvUART_NULL_POINT;

    // if last symbol is null
    if (str[sendIteration[uart]] == '\0') {
        // clear iteration counter
        sendIteration[uart] = 0;

        // return OK code
        return drvUART_OK;
    }

    // Try to transfer byte
    drvUARTErrorCode errCode = halUARTSendByte(uart, (uint8_t)str[sendIteration[uart]]);

    switch (errCode) {
        // if transfer code is OK
        case drvUART_OK:
            // next iteration
            sendIteration[uart]++;
            break;

        default:
            // if error
            return errCode;
    }

    return drvUART_IN_PROGRESS;
}

drvUARTErrorCode drvReceiveDataArray(drvUART uart, uint8_t* dest, uint16_t size) {
    // checking on null
    if (dest == 0)
        return drvUART_NULL_POINT;

    // if received necessary count of byte
    if (receiveIteration[uart] == size) {
        // clear counter
        receiveIteration[uart] = 0;

        // return OK code
        return drvUART_OK;
    }

    // Try to receive byte
    drvUARTErrorCode errCode = halUARTReceiveByte(uart, &dest[receiveIteration[uart]]);
    switch (errCode) {
        // if OK
        case drvUART_OK:
            // next iteration
            receiveIteration[uart]++;
            break;

        // if error
        default:
            return errCode;
    }

    return drvUART_IN_PROGRESS;
}

drvUARTErrorCode drvReceiveString(drvUART uart, char* dest, uint16_t destSize) {
    // checking on null
    if (dest == 0)
        return drvUART_NULL_POINT;

    // if received count equals buffer size
    if (receiveIteration[uart] == destSize) {
        // clear counter
        receiveIteration[uart] = 0;
        // return overload status code
        return drvUART_FULL_BUFF;
    }

    // Try to receive byte
    drvUARTErrorCode errCode = halUARTReceiveByte(uart, (uint8_t*)&dest[receiveIteration[uart]]);
    switch (errCode) {
        // status code is OK
        case drvUART_OK:
            // checking on null symbol
            if(dest[receiveIteration[uart]] == '\0'){
                // clear counter
                receiveIteration[uart] = 0;

                return drvUART_OK;
            }
            // next iteration
            receiveIteration[uart]++;
            break;
        default:
            break;
    }

    return drvUART_IN_PROGRESS;
}
