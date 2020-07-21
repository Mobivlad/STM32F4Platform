/*
 * hal_gpio.h
 *
 *  Created on: 29 трав. 2020 р.
 *      Author: vladyslav.daliavskyi
 */

#ifndef HAL_HAL_GPIO_H_
#define HAL_HAL_GPIO_H_

#include "stm32f7xx.h"

/// Pin states
typedef enum {
    halGPIO_Set   = SET,    /**< Set state */
    halGPIO_Reset = RESET   /**< Reset state */
} halGPIO_pinState;

/// Port's pin enumeration
typedef enum {
    halGPIO_Pin0  = GPIO_PIN_0,
    halGPIO_Pin1  = GPIO_PIN_1,
    halGPIO_Pin2  = GPIO_PIN_2,
    halGPIO_Pin3  = GPIO_PIN_3,
    halGPIO_Pin4  = GPIO_PIN_4,
    halGPIO_Pin5  = GPIO_PIN_5,
    halGPIO_Pin6  = GPIO_PIN_6,
    halGPIO_Pin7  = GPIO_PIN_7,
    halGPIO_Pin8  = GPIO_PIN_8,
    halGPIO_Pin9  = GPIO_PIN_9,
    halGPIO_Pin10 = GPIO_PIN_10,
    halGPIO_Pin11 = GPIO_PIN_11,
    halGPIO_Pin12 = GPIO_PIN_12,
    halGPIO_Pin13 = GPIO_PIN_13,
    halGPIO_Pin14 = GPIO_PIN_14,
    halGPIO_Pin15 = GPIO_PIN_15,
    halGPIO_PinCount
} halGPIO_pin;

/// STM32F407-Disk1 port enumeration
typedef enum {
    halGPIO_PortA = 0,
    halGPIO_PortB,
    halGPIO_PortC,
    halGPIO_PortD,
    halGPIO_PortE,
    halGPIO_PortF,
    halGPIO_PortG,
    halGPIO_PortH,
    halGPIO_PortI,
    halGPIO_PortJ,
    halGPIO_PortK,
    halGPIO_PortCount
} halGPIO_port;

typedef struct {
  GPIO_TypeDef* const   portDef;
  uint32_t              enablePortClockDef;
} halGPIO_portDef_t;

/// Pins functional type enumeration
typedef enum {
    halGPIO_ModeIn,         /**< In type */
    halGPIO_ModeOut,        /**< Out type */
    halGPIO_ModeAF,         /**< Alternative function */
    halGPIO_ModeAn          /**< Analog read-write */
} halGPIO_mode;

/// Pins frequency enumeration
typedef enum {
    halGPIO_Speed2Mhz   = GPIO_SPEED_FREQ_LOW,       /**< 2 MHz */
    halGPIO_Speed25Mhz  = GPIO_SPEED_FREQ_MEDIUM,    /**< 25 MHz */
    halGPIO_Speed50Mhz  = GPIO_SPEED_FREQ_HIGH,      /**< 50 MHz */
    halGPIO_Speed100Mhz = GPIO_SPEED_FREQ_VERY_HIGH  /**< 100 MHz */
} halGPIO_speed;

/// Internal pull-up type enumeration
typedef enum {
    halGPIO_PuPdUp   = GPIO_PULLUP,         /**< Up to VCC. */
    halGPIO_PuPdDown = GPIO_PULLDOWN,       /**< Down to Ground. */
    halGPIO_PuPdNo   = GPIO_NOPULL,         /**< Not to ground, not to power. */
} halGPIO_PuPd;

/// Pin's out type enumeration
typedef enum {
    halGPIO_OTPP,  /**< Push-pull type. */
    halGPIO_OTOD   /**< Open-drain type. */
} halGPIO_OType;

/// GPIO pin initial structure
typedef struct {
    GPIO_InitTypeDef    pinInitStruct;

    halGPIO_port        port;
    halGPIO_pinState    state;     /**< Show current pint state.
                                         Used only for out type */
} halGPIO_struct;

/// initial parameters structure
typedef struct {
    halGPIO_pin     pin;          /**< Configured pin number. */
    halGPIO_mode    mode;         /**< Pin work mode. */
    halGPIO_speed   speed;        /**< Pin frequency */
    halGPIO_OType   outType;      /**< Output states */
    halGPIO_PuPd    PuPd;         /**< Push-pull direction */
} halGPIO_initStruct;

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
