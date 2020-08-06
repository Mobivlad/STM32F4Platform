/*
 * bl_adc_file_write.h
 *
 *  Created on: Aug 5, 2020
 *      Author: Sigma
 */

#ifndef BL_BL_ADC_FILE_WRITER_H_
#define BL_BL_ADC_FILE_WRITER_H_

#include "ul_fat_fs.h"

#include "stdio.h"

#define BUFFER_SIZE             50

#define FOLDER_PREFIX           "adcData"
#define FILE_PREFIX             "file"
#define FILE_FORMAT             ".csv"

#define FILE_NAME_MASE          "%s/%s%d"FILE_FORMAT

#define FIND_FOLDER_PREFIX      FOLDER_PREFIX"*"
#define FOLDER_NAME_MASK        "%s%d"

#define ARRAY_SIZE(X)           (sizeof(x) / sizeof(x[0]))

#define MAX_VOLTAGE             3.3f
#define MAX_ADC_VAL             0x0FFF

typedef struct {

    ulFatFS_struct      fatFS;
    uint16_t*           adcValue;
    uint8_t             fileNumber;
    SemaphoreHandle_t   mutex;

    ulFatFS_File_t*     currentFile;

    char fileNameBuffer[BUFFER_SIZE];
    char folderNameBuffer[BUFFER_SIZE];
    char dataStrBuffer[BUFFER_SIZE];
} blADCFileWriter_struct;

typedef enum {
    blADCFW_OK = 0,
    blADCFW_EROOR,
    blADCFW_NULL_POINT
} blADCFW_error;

blADCFW_error blADCFileWriterInit(blADCFileWriter_struct* fileWriterStruct, SemaphoreHandle_t writeMutex, uint16_t* valuePtr);


#endif /* BL_BL_ADC_FILE_WRITER_H_ */
