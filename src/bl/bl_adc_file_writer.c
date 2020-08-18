/*
 * bl_adc_file_writer.c
 *
 *  Created on: Aug 5, 2020
 *      Author: Sigma
 */

#include "bl_adc_file_writer.h"

static blADCFW_error initSaveFolder(blADCFileWriter_struct* fileWriterStruct) {
    uint8_t foldersNumber = 0;
    if (ulFatFSFindCount(&fileWriterStruct->fatFS, "", FIND_FOLDER_PREFIX, ulFatFS_Folder,
            &foldersNumber) != ulFatFS_OK) {
        return blADCFW_EROOR;
    }
    sprintf(fileWriterStruct->folderNameBuffer, FOLDER_NAME_MASK, FOLDER_PREFIX, foldersNumber+1);
    if (ulFatFSCreateFolder(&fileWriterStruct->fatFS, fileWriterStruct->folderNameBuffer)
            != ulFatFS_OK) {
        return blADCFW_EROOR;
    }
    return blADCFW_OK;
}

blADCFW_error blADCFileWriterInit(blADCFileWriter_struct* fileWriterStruct, QueueHandle_t writeValues) {
    if (fileWriterStruct == NULL) return blADCFW_NULL_POINT;

    fileWriterStruct->adcValues = writeValues;

    if (ulFatFSInit(&fileWriterStruct->fatFS) != ulFatFS_OK) {
        return blADCFW_EROOR;
    }

    if (ulFatFSMountSD(&fileWriterStruct->fatFS) != ulFatFS_OK) {
        return blADCFW_EROOR;
    }
    if (initSaveFolder(fileWriterStruct) != blADCFW_OK) {
        return blADCFW_EROOR;
    }
    fileWriterStruct->fileNumber    = 0;

    return blADCFW_OK;
}

void blADCInitFileName(blADCFileWriter_struct* fileWriterStruct) {
    sprintf(fileWriterStruct->fileNameBuffer, FILE_NAME_MASE, fileWriterStruct->folderNameBuffer,
            FILE_PREFIX, fileWriterStruct->fileNumber + 1);
}

static blADCFW_bufferCopyError clearBuffer(blADCFileWriter_struct* fileWriterStruct) {
    memset(fileWriterStruct->fileWriteData, 0, WRITE_BUFFER_SIZE);
    fileWriterStruct->index = 0;
    return blADCFW_COPY_OK;
}

blADCFW_error blADCOpenFile(blADCFileWriter_struct* fileWriterStruct) {
    if (fileWriterStruct == NULL) {
        return blADCFW_NULL_POINT;
    }

    clearBuffer(fileWriterStruct);

    blADCInitFileName(fileWriterStruct);
    if (ulFatFSOpenFile(&fileWriterStruct->fatFS, &fileWriterStruct->currentFile,
            fileWriterStruct->fileNameBuffer, OW) != ulFatFS_OK) {
        return blADCFW_EROOR;
    }
    return blADCFW_OK;
}

static blADCFW_error pushBuffer(blADCFileWriter_struct* fileWriterStruct) {
    if (ulFatFSWriteString(&fileWriterStruct->fatFS, &fileWriterStruct->currentFile,
            (const char*) fileWriterStruct->fileWriteData) != ulFatFS_OK) {
        return blADCFW_EROOR;
    }
    return blADCFW_OK;
}

blADCFW_error blADCCloseFile(blADCFileWriter_struct* fileWriterStruct) {
    if (fileWriterStruct == NULL) {
        return blADCFW_NULL_POINT;
    }

    if (fileWriterStruct->index != 0) {
        if (pushBuffer(fileWriterStruct) != blADCFW_OK) {
            return blADCFW_EROOR;
        }
    }

    if (ulFatFSCloseFile(&fileWriterStruct->fatFS, &fileWriterStruct->currentFile) != ulFatFS_OK) {
        return blADCFW_EROOR;
    }

    fileWriterStruct->fileNumber++;
    return blADCFW_OK;
}

static blADCFW_bufferCopyError addToBuffer(blADCFileWriter_struct* fileWriterStruct, char* data) {
    uint16_t dataLen = strlen(data);
    if (fileWriterStruct->index + dataLen > WRITE_BUFFER_SIZE - 1) {
        return blADCFW_COPY_OVERLOAD;
    }
    memcpy(&fileWriterStruct->fileWriteData[fileWriterStruct->index], data, dataLen);
    fileWriterStruct->index += dataLen;

    return blADCFW_COPY_OK;
}

static blADCFW_error blADCWriteValue(blADCFileWriter_struct* fileWriterStruct, float value) {
    if (fileWriterStruct == NULL) {
        return blADCFW_NULL_POINT;
    }
    memset(fileWriterStruct->dataStrBuffer, 0, sizeof(fileWriterStruct->dataStrBuffer));
    sprintf(fileWriterStruct->dataStrBuffer, "%.2f\n", value);

    if (addToBuffer(fileWriterStruct, fileWriterStruct->dataStrBuffer) != blADCFW_COPY_OK) {
        if (pushBuffer(fileWriterStruct) != blADCFW_OK) {
            return blADCFW_EROOR;
        }
        clearBuffer(fileWriterStruct);
        addToBuffer(fileWriterStruct, fileWriterStruct->dataStrBuffer);
    }

    return blADCFW_OK;
}

void blADCFileWriterTask(void* parametr) {
    blADCFileWriter_struct* fileWriterStruct = (blADCFileWriter_struct*) parametr;
    while(1) {
        uint16_t adcValue;
        xQueueReceive(fileWriterStruct->adcValues, &adcValue, portMAX_DELAY);

        float value = (float)adcValue * MAX_VOLTAGE / MAX_ADC_VAL;
        blADCWriteValue(fileWriterStruct, value);
    }
}
