/*
 * hal_gpio.h
 *
 *  Created on: 29 трав. 2020 р.
 *      Author: vladyslav.daliavskyi
 */

#ifndef HAL_HAL_GPIO_H_
#define HAL_HAL_GPIO_H_

#include "stm32f4xx.h"

/// Pin states
typedef enum {
    halGPIO_Set   = SET,    /**< Set state */
    halGPIO_Reset = RESET   /**< Reset state */
} halGPIO_pinState;

/// Port's pin enumeration
typedef enum {
    halGPIO_Pin0  = GPIO_Pin_0,
    halGPIO_Pin1  = GPIO_Pin_1,
    halGPIO_Pin2  = GPIO_Pin_2,
    halGPIO_Pin3  = GPIO_Pin_3,
    halGPIO_Pin4  = GPIO_Pin_4,
    halGPIO_Pin5  = GPIO_Pin_5,
    halGPIO_Pin6  = GPIO_Pin_6,
    halGPIO_Pin7  = GPIO_Pin_7,
    halGPIO_Pin8  = GPIO_Pin_8,
    halGPIO_Pin9  = GPIO_Pin_9,
    halGPIO_Pin10 = GPIO_Pin_10,
    halGPIO_Pin11 = GPIO_Pin_11,
    halGPIO_Pin12 = GPIO_Pin_12,
    halGPIO_Pin13 = GPIO_Pin_13,
    halGPIO_Pin14 = GPIO_Pin_14,
    halGPIO_Pin15 = GPIO_Pin_15,
    halGPIO_PinCount
} halGPIO_pin;

/// STM32F407-Disk1 port enumeration
typedef enum {
    halGPIO_PortA = 0,
    halGPIO_PortB,
    halGPIO_PortC,
    halGPIO_PortD,
    halGPIO_PortE,
    halGPIO_PortCount
} halGPIO_port;

/// Pins functional type enumeration
typedef enum {
    halGPIO_ModeIn  = GPIO_Mode_IN,     /**< In type */
    halGPIO_ModeOut = GPIO_Mode_OUT,    /**< Out type */
    halGPIO_ModeAF  = GPIO_Mode_AF,     /**< Alternative function */
    halGPIO_ModeAn  = GPIO_Mode_AN      /**< Analog read-write */
} halGPIO_mode;

/// Pins frequency enumeration
typedef enum {
    halGPIO_Speed2Mhz   = GPIO_Low_Speed,       /**< 2 MHz */
    halGPIO_Speed25Mhz  = GPIO_Medium_Speed,    /**< 25 MHz */
    halGPIO_Speed50Mhz  = GPIO_High_Speed,      /**< 50 MHz */
    halGPIO_Speed100Mhz = GPIO_Fast_Speed       /**< 100 MHz */
} halGPIO_speed;

/// Internal pull-up type enumeration
typedef enum {
    halGPIO_PuPdUp   = GPIO_PuPd_UP,         /**< Up to VCC. */
    halGPIO_PuPdDown = GPIO_PuPd_DOWN,       /**< Down to Ground. */
    halGPIO_PuPdNo   = GPIO_PuPd_NOPULL,     /**< Not to ground, not to power. */
} halGPIO_PuPd;

/// Pin's out type enumeration
typedef enum {
    halGPIO_OTPP = GPIO_OType_PP,  /**< Push-pull type. */
    halGPIO_OTOD = GPIO_OType_OD   /**< Open-drain type. */
} halGPIO_OType;

/// GPIO pin initial structure
typedef struct {
    GPIO_InitTypeDef pinInitStruct;

    halGPIO_port port;
    halGPIO_pinState state;     /**< Show current pint state.
                                         Used only for out type */
} halGPIO_struct;

/// initial parameters structure
typedef struct {
    halGPIO_pin pin;            /**< Configured pin number. */
    halGPIO_mode mode;          /**< Pin work mode. */
    halGPIO_speed speed;        /**< Pin frequency */
    halGPIO_OType outType;      /**< Output states */
    halGPIO_PuPd PuPd;          /**< Push-pull direction */
} halGPIO_initStruct;

typedef struct {
  GPIO_TypeDef* const portDef;
  uint32_t portClock;

} halGPIO_portTable;

/**
 * Configured GPIO function.
 * @param gpioStruct pointer on structure that consist main configuration
 */
void halGPIOInit(halGPIO_struct* const gpioStruct, const halGPIO_initStruct* const initStruct);

/**
 * Set high level on gpio pin.
 * @param gpioStruct pointer on structure with data
 */
void halGPIOSetPins(halGPIO_struct* const gpioStruct);

/**
 * Set low level on gpio pin.
 * @param gpioStruct pointer on structure with data
 */
void halGPIOResetPins(halGPIO_struct* const gpioStruct);

/**
 * Toggle level on gpio pin.
 * @param gpioStruct pointer on structure with data
 */
void halGPIOTogglePins(halGPIO_struct* const gpioStruct);

/**
 * Function that return current pin state.
 * @param gpioStruct pointer on structure with data
 * @return current pin state from halGPIO_pinState enumeration
 */
halGPIO_pinState halGPIOReadPin(const halGPIO_struct* const gpioStruct);

#endif /* HAL_HAL_GPIO_H_ */
