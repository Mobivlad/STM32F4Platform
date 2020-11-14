/*
 * ul_fat_fs.c
 *
 *  Created on: Jul 29, 2020
 *      Author: Sigma
 */

#include "ul_fat_fs.h"

static ulFatFS_struct* current_fatfs;

/*
 * Adapter block
 */

static volatile DSTATUS Stat = STA_NOINIT;

DSTATUS USBH_status(BYTE lun)
{
  DRESULT res = RES_ERROR;

  if(USBH_MSC_UnitIsReady(&current_fatfs->usb->usbHost, lun))
  {
    res = RES_OK;
  }
  else
  {
    res = RES_ERROR;
  }

  return res;
}

static DSTATUS USBH_initialize(BYTE lun) {
    return RES_OK;
}

static DRESULT USBH_read(BYTE lun, BYTE *buff, DWORD sector, UINT count) {
  DRESULT res = RES_ERROR;
    MSC_LUNTypeDef info;

    if(USBH_MSC_Read(&current_fatfs->usb->usbHost, lun, sector, buff, count) == USBH_OK)
    {
      res = RES_OK;
    }
    else
    {
      USBH_MSC_GetLUNInfo(&current_fatfs->usb->usbHost, lun, &info);

      switch (info.sense.asc)
      {
      case SCSI_ASC_LOGICAL_UNIT_NOT_READY:
      case SCSI_ASC_MEDIUM_NOT_PRESENT:
      case SCSI_ASC_NOT_READY_TO_READY_CHANGE:
        USBH_ErrLog ("USB Disk is not ready!");
        res = RES_NOTRDY;
        break;

      default:
        res = RES_ERROR;
        break;
      }
    }

    return res;
}

DRESULT USBH_write(BYTE lun, const BYTE *buff, DWORD sector, UINT count)
{
  DRESULT res = RES_ERROR;
  MSC_LUNTypeDef info;

  if(USBH_MSC_Write(&current_fatfs->usb->usbHost, lun, sector, (BYTE *)buff, count) == USBH_OK)
  {
    res = RES_OK;
  }
  else
  {
    USBH_MSC_GetLUNInfo(&current_fatfs->usb->usbHost, lun, &info);

    switch (info.sense.asc)
    {
    case SCSI_ASC_WRITE_PROTECTED:
      USBH_ErrLog("USB Disk is Write protected!");
      res = RES_WRPRT;
      break;

    case SCSI_ASC_LOGICAL_UNIT_NOT_READY:
    case SCSI_ASC_MEDIUM_NOT_PRESENT:
    case SCSI_ASC_NOT_READY_TO_READY_CHANGE:
      USBH_ErrLog("USB Disk is not ready!");
      res = RES_NOTRDY;
      break;

    default:
      res = RES_ERROR;
      break;
    }
  }

  return res;
}

static DRESULT USBH_ioctl(BYTE lun, BYTE cmd, void *buff) {
  DRESULT res = RES_ERROR;
    MSC_LUNTypeDef info;

    switch (cmd)
    {
    /* Make sure that no pending write process */
    case CTRL_SYNC:
      res = RES_OK;
      break;

    /* Get number of sectors on the disk (DWORD) */
    case GET_SECTOR_COUNT :
      if(USBH_MSC_GetLUNInfo(&current_fatfs->usb->usbHost, lun, &info) == USBH_OK)
      {
        *(DWORD*)buff = info.capacity.block_nbr;
        res = RES_OK;
      }
      else
      {
        res = RES_ERROR;
      }
      break;

    /* Get R/W sector size (WORD) */
    case GET_SECTOR_SIZE :
      if(USBH_MSC_GetLUNInfo(&current_fatfs->usb->usbHost, lun, &info) == USBH_OK)
      {
        *(DWORD*)buff = info.capacity.block_size;
        res = RES_OK;
      }
      else
      {
        res = RES_ERROR;
      }
      break;

      /* Get erase block size in unit of sector (DWORD) */
    case GET_BLOCK_SIZE :

      if(USBH_MSC_GetLUNInfo(&current_fatfs->usb->usbHost, lun, &info) == USBH_OK)
      {
        *(DWORD*)buff = info.capacity.block_size / SD_BLOCK_SIZE;
        res = RES_OK;
      }
      else
      {
        res = RES_ERROR;
      }
      break;

    default:
      res = RES_PARERR;
    }

    return res;
}

static const Diskio_drvTypeDef  USBH_Driver =
{
    USBH_initialize, USBH_status, USBH_read, USBH_write, USBH_ioctl
};


/*
 * End adapter block
 */

ulFatFS_error ulFatFSInit(ulFatFS_struct* fatfs, ulFatFS_initStruct* initStruct) {
    fatfs->usb = initStruct->usbStruct;
    current_fatfs = fatfs;

    return FATFS_LinkDriver(&USBH_Driver) == 0 ? ulFatFS_OK : ulFatFS_DISK_ERR;
}

ulFatFS_error ulFatFSMount(ulFatFS_struct* fatfs) {
    if (fatfs == NULL) {
        return ulFatFS_DATA_NULL_POINT;
    }

    FRESULT functionRes = f_mount(&current_fatfs->FatFs, "", 0);

    return functionRes;
}

ulFatFS_error ulFatFSUnmount(ulFatFS_struct* fatfs) {
    if (fatfs == NULL) {
        return ulFatFS_DATA_NULL_POINT;
    }

    FRESULT functionRes = f_mount(NULL, "", 0);

    return functionRes;
}

ulFatFS_error ulFatFSCreateFolder(ulFatFS_struct* fatfs, char* folderName) {
    if (fatfs == NULL) {
        return ulFatFS_DATA_NULL_POINT;
    }

    FRESULT functionRes = f_mkdir(folderName);

    return functionRes;
}

ulFatFS_error ulFatFSFindCount(ulFatFS_struct* fatfs, char* folderPath, char* fileMask,
        ulFatFS_elementType elementType, uint8_t* count) {
    if (fatfs == NULL || folderPath == NULL || fileMask == NULL || count == NULL) {
        return ulFatFS_DATA_NULL_POINT;
    }

    DIR dir;
    FILINFO fno;
    uint8_t tmp_count = 0;
    FRESULT fr = f_findfirst(&dir, &fno, folderPath, fileMask);
    switch (fr) {
        case FR_NO_PATH: {
            *count = 0;
            fr = FR_OK;
            break;
        }
        case FR_OK: {
            while (*(fno.fname)) {
                if (fr != (FRESULT) ulFatFS_OK) {

                    return fr;
                }
                switch (elementType) {
                    case ulFatFS_Folder:
                        if (fno.fattrib & AM_DIR) {
                            tmp_count++;
                        }
                        break;
                    case ulFatFS_File:
                        if (!(fno.fattrib & AM_DIR)) {
                            tmp_count++;
                        }
                        break;
                }
                fr = f_findnext(&dir, &fno);
            }
            fr = f_closedir(&dir);
            *count = (fr == (FRESULT) ulFatFS_OK) ? tmp_count : SD_ERROR_VALUE;
            break;
        }
        default: {
            break;
        }
    }

    return fr;

}

ulFatFS_error ulFatFSOpenFile(ulFatFS_struct* fatfs, ulFatFS_File_t* file, const char* fileName,
        ulFatFS_FileOpenAttribute_t posix) {
    if (fatfs == NULL) {
        return ulFatFS_DATA_NULL_POINT;
    }

    FRESULT functionRes = f_open(file, fileName, posix);

    return functionRes;
}

ulFatFS_error ulFatFSWrite(ulFatFS_struct* fatfs, ulFatFS_File_t* file, const uint8_t* data,
        uint16_t dataLen) {
    if (fatfs == NULL || data == NULL || file == NULL) {
        return ulFatFS_DATA_NULL_POINT;
    }

    uint16_t writtenBytes = 0;
    FRESULT fr = f_write(file, data, dataLen, (unsigned int*) &writtenBytes);
    if (fr != (FRESULT) ulFatFS_OK) {

        return fr;
    }

    return writtenBytes == dataLen ? ulFatFS_OK : ulFatFS_COMPLETE_OPERATION_ERROR;
}

ulFatFS_error ulFatFSWriteString(ulFatFS_struct* fatfs, ulFatFS_File_t* file, const char* str) {
    if (fatfs == NULL || str == NULL || file == NULL) {
        return ulFatFS_DATA_NULL_POINT;
    }

    uint16_t writtenBytes = 0;
    uint16_t dataLen = strlen(str);
    FRESULT fr = f_write(file, str, dataLen, (unsigned int*) &writtenBytes);
    if (fr != (FRESULT) ulFatFS_OK) {

        return fr;
    }

    return writtenBytes == dataLen ? ulFatFS_OK : ulFatFS_COMPLETE_OPERATION_ERROR;
}

ulFatFS_error ulFatFSRead(ulFatFS_struct* fatfs, ulFatFS_File_t* file, uint8_t* data,
        uint16_t dataLen) {
    if (fatfs == NULL || data == NULL || file == NULL) {
        return ulFatFS_DATA_NULL_POINT;
    }

    uint16_t readBytes = 0;
    FRESULT fr = f_read(file, data, dataLen, (unsigned int*) &readBytes);
    if (fr != (FRESULT) ulFatFS_OK) {

        return fr;
    }

    return readBytes == dataLen ? ulFatFS_OK : ulFatFS_COMPLETE_OPERATION_ERROR;

}

ulFatFS_error ulFatFSCloseFile(ulFatFS_struct* fatfs, ulFatFS_File_t* file) {
    if (fatfs == NULL || file == NULL) {
        return ulFatFS_DATA_NULL_POINT;
    }

    FRESULT fr = f_close(file);

    return fr;
}

