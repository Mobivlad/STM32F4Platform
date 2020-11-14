#include "bl_file_rewriter.h"

static blFileWriter_struct* currentFileWriter; 

static void onConnect() {
  ulFatFSMount(&currentFileWriter->fatFS);
  drvLedOn(&currentFileWriter->connectLed);
  ulFatFS_File_t ReadFile;
  ulFatFS_File_t WriteFile;
  uint8_t fileCount;
  ulFatFSFindCount(&currentFileWriter->fatFS, "", FIND_FILE_NAME, ulFatFS_File, &fileCount);
  if (fileCount == 0) {
    // create file with default text
    ulFatFSOpenFile(&currentFileWriter->fatFS, &WriteFile, FIND_FILE_NAME, OW);
    ulFatFSWriteString(&currentFileWriter->fatFS, &WriteFile, DEFAULT_TEXT);
    ulFatFSCloseFile(&currentFileWriter->fatFS, &WriteFile);
  } else {
    // rewrite file to other file
    ulFatFSOpenFile(&currentFileWriter->fatFS, &ReadFile, FIND_FILE_NAME, R);
    ulFatFSOpenFile(&currentFileWriter->fatFS, &WriteFile, NEW_FILE_NAME, OW);
    char tmp_text[BUFFER_SIZE];
    do {
      ulFatFSRead(&currentFileWriter->fatFS, &ReadFile, (uint8_t*)tmp_text, BUFFER_SIZE);
      ulFatFSWriteString(&currentFileWriter->fatFS, &WriteFile, tmp_text);
    } while (tmp_text[BUFFER_SIZE-1] == 0);
    ulFatFSCloseFile(&currentFileWriter->fatFS, &ReadFile);
    ulFatFSCloseFile(&currentFileWriter->fatFS, &WriteFile);
  }
  drvLedOn(&currentFileWriter->doneLed);
}

static void onDisconnect() {
  ulFatFSUnmount(&currentFileWriter->fatFS);
  drvLedOff(&currentFileWriter->doneLed);
  drvLedOff(&currentFileWriter->connectLed);
}

void blFileRewriterInit(blFileWriter_struct* fileRewriterStruct) {
  currentFileWriter = fileRewriterStruct;
  ulFatFS_initStruct initFatFS;
  initFatFS.usbStruct = &fileRewriterStruct->usb;
  ulFatFSInit(&fileRewriterStruct->fatFS, &initFatFS);

  drvUSBMSC_initStruct initUSB;
  initUSB.onReady = onConnect;
  initUSB.onDisconnect = onDisconnect;
  drvUSBMSCInit(&fileRewriterStruct->usb, &initUSB);

  fileRewriterStruct->connectLed.led        = drvLed2;
  fileRewriterStruct->connectLed.controlPin = drvLed_ControlPin_Anode;
  fileRewriterStruct->connectLed.state      = drvLed_Off;
  drvLedInit(&fileRewriterStruct->connectLed);

  fileRewriterStruct->doneLed.led         = drvLed3;
  fileRewriterStruct->doneLed.controlPin  = drvLed_ControlPin_Anode;
  fileRewriterStruct->doneLed.state       = drvLed_Off;
  drvLedInit(&fileRewriterStruct->doneLed);
}

void blFileRewriterRun(blFileWriter_struct* fileRewriterStruct) {
  drvUSBMSCRun(&fileRewriterStruct->usb);
}

void ulFileRewriterTaskFunction(void* parametr) {
    blFileWriter_struct* const blFileRewriter = (blFileWriter_struct* const ) parametr;
    while (1) {
        blFileRewriterRun(blFileRewriter);
    }
}