/*
 * bl_adc_writer.h
 *
 *  Created on: Aug 7, 2020
 *      Author: Sigma
 */

#ifndef BL_BL_ADC_WRITER_H_
#define BL_BL_ADC_WRITER_H_

#include "FreeRTOS.h"
#include "semphr.h"

#include "drv_usb_cdc.h"
#include <stdio.h>
#include <string.h>

#define ADC_MAX_FREQ        1000

#define UIN16_MAX           0xFFFF

#define MAX_ADC_VAL         0x0FFF

#define WRITER_QUEUE_SIZE   200
#define CHAR_BUFFER_SIZE    5     

#define FLOAT_TO_CHAR_MASK  "%4.2f"

typedef enum {
    blADCWriter_Command = 0 // remove it when add new command
} blADCWriter_command;

typedef enum {
    blADCWriter_DATA = 0,
    blADCWriter_COMMAND
} blADCWriter_recordType;

typedef struct {
    blADCWriter_recordType  recordType;
    float                   data;
} blADCWriter_record;

typedef enum {
    blADCWriter_OK = 0,
    blADCWriter_EROOR,
    blADCWriter_NULL_POINT
} blADCWriter_error;

typedef struct {
    QueueHandle_t       writerDataQueue;

    drvUSBCDC_struct    usb;
} blADCWriter_struct;

blADCWriter_error blADCWriterInit(blADCWriter_struct* writerStruct);
void blADCWriterTask(void* parametr);


#endif /* BL_BL_ADC_WRITER_H_ */
