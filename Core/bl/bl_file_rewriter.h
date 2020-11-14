#ifndef __BL_BL_FILE_REWRITER__
#define __BL_BL_FILE_REWRITER__

#include "drv_usb_msc.h"
#include "ul_fat_fs.h"
#include "drv_led.h"

#define FIND_FILE_NAME  "file.txt"
#define NEW_FILE_NAME  "file_n.txt"
#define DEFAULT_TEXT    "hello world"
#define BUFFER_SIZE     255

typedef struct {
  ulFatFS_struct    fatFS;
  drvUSBMSC_struct  usb;
  drvLed_struct     connectLed;
  drvLed_struct     doneLed;
} blFileWriter_struct;

void blFileRewriterInit(blFileWriter_struct* fileRewriterStruct);

void blFileRewriterRun(blFileWriter_struct* fileRewriterStruct);
void ulFileRewriterTaskFunction(void* parametr);


#endif //__BL_BL_FILE_REWRITER__