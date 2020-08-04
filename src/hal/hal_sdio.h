/*
 * hal_sdio.h
 *
 *  Created on: Jul 23, 2020
 *      Author: Sigma
 */

#ifndef HAL_HAL_SDIO_H_
#define HAL_HAL_SDIO_H_

#include "stm32f7xx.h"

#define SD_DATATIMEOUT                              ((uint32_t)100000000)

typedef enum {
    halSDIO1 = 0,
    halSDIO2,
    halSDIO_Count
} halSDIO;

typedef enum {
    halSDIO_TRANSFER_OK = 0,
    halSDIO_TRANSFER_BUSY
} halSDIO_state;

typedef enum {
    halSDIO_Present = 0,
    halSDIO_NotPresent,
} halSDIO_SDPresentState;

typedef struct {
    GPIO_TypeDef *  port;
    uint32_t        pin;
    uint32_t        port_rcc;
    uint8_t         af;
} halSDIO_pinDef_t;

#define WIDE_WIDTH_4  4
#define WIDE_WIDTH_1  1

typedef struct {
    SDMMC_TypeDef *  instance;
    uint32_t         rcc;
    halSDIO_pinDef_t ck;
    halSDIO_pinDef_t cmd;
    halSDIO_pinDef_t d[WIDE_WIDTH_4];
    halSDIO_pinDef_t detectPin;
} halSDIO_def_t;

typedef enum {
    halSDIO_pinSDIO,
    halSDIO_pinDetect
} halSDIO_pinType;

typedef enum {
    halSDIO_1Bit    = SDMMC_BUS_WIDE_1B,
    halSDIO_4Bits   = SDMMC_BUS_WIDE_4B
} halSDIO_wideWigth;

typedef enum {
    halSDIO_Hard = GPIO_NOPULL,
    halSDIO_Soft = GPIO_PULLUP
} halSDIO_PuPdType;

typedef enum {
    halSDIO_Rising  = SDMMC_CLOCK_EDGE_RISING,
    halSDIO_Falling = SDMMC_CLOCK_EDGE_FALLING
} halSDIO_clockTransitionEdge;

typedef enum {
    halSDIO_clockBypassEnable  = SDMMC_CLOCK_BYPASS_ENABLE,
    halSDIO_clockBypassDisable = SDMMC_CLOCK_BYPASS_DISABLE
} halSDIO_clockBypass;

typedef enum {
    halSDIO_clockPowerSaveEnable  = SDMMC_CLOCK_POWER_SAVE_ENABLE,
    halSDIO_clockPowerSaveDisable = SDMMC_CLOCK_POWER_SAVE_DISABLE
} halSDIO_clockPowerSave;

typedef enum {
    halSDIO_hardFlowControlEnable  = SDMMC_HARDWARE_FLOW_CONTROL_ENABLE,
    halSDIO_hardFlowControlDisable = SDMMC_HARDWARE_FLOW_CONTROL_DISABLE
} halSDIO_hardFlowControl;

typedef struct {
    halSDIO_wideWigth           wideWigth;
    halSDIO_PuPdType            ppType;
    halSDIO_clockTransitionEdge clockEdge;
    halSDIO_clockBypass         clockBypass;
    halSDIO_clockPowerSave      powerSave;
    halSDIO_hardFlowControl     flowControl;
    uint8_t                     clockDiv;
} halSDIO_initStruct;

typedef enum
{
  halSDIO_OK       = HAL_OK,
  halSDIO_ERROR    = HAL_ERROR,
  halSDIO_BUSY     = HAL_BUSY,
  halSDIO_TIMEOUT  = HAL_TIMEOUT,
  halSDIO_SD_NOT_PRESENT
} halSDIO_error;

typedef struct {
    SD_HandleTypeDef            sdHandle;
    HAL_SD_CardStatusTypeDef    sdStatus;
    HAL_SD_CardInfoTypeDef      sdInfo;
    halSDIO                     sdio;
} halSDIO_struct;

halSDIO_error halSDIOInit(halSDIO_struct* sdio_struct, halSDIO_initStruct* initStruct);

halSDIO_state halSDIOGetCardState(halSDIO_struct* sdio_struct);
halSDIO_error halSDIOGetStatus(halSDIO_struct* sdio_struct);
void halSDIOGetCardInfo(halSDIO_struct* sdio_struct);

halSDIO_error halSDIOWriteBlocks(halSDIO_struct* sdio_struct, const uint8_t *pData, uint32_t blockAdd, uint32_t blocksNumber, uint32_t timeout);
halSDIO_error halSDIOReadBlocks(halSDIO_struct* sdio_struct, uint8_t *pData, uint32_t blockAdd, uint32_t blocksNumber, uint32_t timeout);
#endif /* HAL_HAL_SDIO_H_ */
