/*
 * bl_adc_display.c
 *
 *  Created on: Aug 7, 2020
 *      Author: Sigma
 */

#include "bl_adc_writer.h"

blADCWriter_error blADCWriterInit(blADCWriter_struct* writerStruct) {
    if (writerStruct == NULL) {
        return blADCWriter_NULL_POINT;
    }

    writerStruct->writerDataQueue = xQueueCreate(WRITER_QUEUE_SIZE, sizeof(blADCWriter_record));

    drvUSBCDCInit(&writerStruct->usb, NULL);

    return blADCWriter_OK;
}

/**
 * Function to convert float value to string and save in buffer
 * @param x float value to convert
 * @return 0 if success, else - error
 */
static uint8_t convertToChar(float x, char* buffer) {
  if (buffer == NULL) return -1;
  memset(buffer, 0, CHAR_BUFFER_SIZE);
  if (sprintf(buffer, FLOAT_TO_CHAR_MASK, x) == 1) {
    return 0;
  }
  return -1;
}

void blADCWriterTask(void* parametr) {
    blADCWriter_struct* writerStruct = (blADCWriter_struct*) parametr;
    char bufferForStrData[CHAR_BUFFER_SIZE];
    while (1) {
        blADCWriter_record record;
        xQueueReceive(writerStruct->writerDataQueue, &record, portMAX_DELAY);
        if (record.recordType == blADCWriter_DATA) {
            if (convertToChar(record.data, bufferForStrData) == 0) {
              drvUSBCDCWriteStr(&writerStruct->usb, bufferForStrData);
            }   
        } else if (record.recordType == blADCWriter_COMMAND) {
            switch (record.recordType) {
                default:  
                  break;
            }
        }
    }
}
