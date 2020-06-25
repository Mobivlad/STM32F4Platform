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
    halGPIO_Set = SET,      /**< Set state */
    halGPIO_Reset = RESET   /**< Reset state */
} halGPIO_pinState;

/// Port's pin enumeration
typedef enum {
    halGPIO_Pin_0 = GPIO_Pin_0,     /**< Pin 0 */
    halGPIO_Pin_1 = GPIO_Pin_1,     /**< Pin 1 */
    halGPIO_Pin_2 = GPIO_Pin_2,     /**< Pin 2 */
    halGPIO_Pin_3 = GPIO_Pin_3,     /**< Pin 3 */
    halGPIO_Pin_4 = GPIO_Pin_4,     /**< Pin 4 */
    halGPIO_Pin_5 = GPIO_Pin_5,     /**< Pin 5 */
    halGPIO_Pin_6 = GPIO_Pin_6,     /**< Pin 6 */
    halGPIO_Pin_7 = GPIO_Pin_7,     /**< Pin 7 */
    halGPIO_Pin_8 = GPIO_Pin_8,     /**< Pin 8 */
    halGPIO_Pin_9 = GPIO_Pin_9,     /**< Pin 9 */
    halGPIO_Pin_10 = GPIO_Pin_10,   /**< Pin 10 */
    halGPIO_Pin_11 = GPIO_Pin_11,   /**< Pin 11 */
    halGPIO_Pin_12 = GPIO_Pin_12,   /**< Pin 12 */
    halGPIO_Pin_13 = GPIO_Pin_13,   /**< Pin 13 */
    halGPIO_Pin_14 = GPIO_Pin_14,   /**< Pin 14 */
    halGPIO_Pin_15 = GPIO_Pin_15,   /**< Pin 15 */
    halGPIO_PinCount
} halGPIO_pin;

/// STM32F407-Disk1 port enumeration
typedef enum {
    halGPIO_PortA = 0,  /**< Port A */
    halGPIO_PortB,      /**< Port B */
    halGPIO_PortC,      /**< Port C */
    halGPIO_PortD,      /**< Port D */
    halGPIO_PortE,      /**< Port E */
    halGPIO_PortCount
} halGPIO_port;

/// Pins functional type enumeration
typedef enum {
    halGPIO_Mode_In = GPIO_Mode_IN,     /**< In type */
    halGPIO_Mode_Out = GPIO_Mode_OUT,   /**< Out type */
    halGPIO_Mode_AF = GPIO_Mode_AF,     /**< Alternative function */
    halGPIO_Mode_An = GPIO_Mode_AN      /**< Analog read-write */
} halGPIO_mode;

/// Pins frequency enumeration
typedef enum {
    halGPIO_Speed_2Mhz = GPIO_Low_Speed,        /**< 2 MHz */
    halGPIO_Speed_25Mhz = GPIO_Medium_Speed,    /**< 25 MHz */
    halGPIO_Speed_50Mhz = GPIO_High_Speed,      /**< 50 MHz */
    halGPIO_Speed_100Mhz = GPIO_Fast_Speed      /**< 100 MHz */
} halGPIO_speed;

/// Internal pull-up type enumeration
typedef enum {
    halGPIO_PuPd_Up = GPIO_PuPd_UP,         /**< Up to VCC. */
    halGPIO_PuPd_Down = GPIO_PuPd_DOWN,     /**< Down to Ground. */
    halGPIO_PuPd_No = GPIO_PuPd_NOPULL,     /**< Not to ground, not to power. */
} halGPIO_PuPd;

/// Pin's out type enumeration
typedef enum {
    halGPIO_OT_PP = GPIO_OType_PP,  /**< Push-pull type. */
    halGPIO_OT_OD = GPIO_OType_OD   /**< Open-drain type. */
} halGPIO_OType;

/// GPIO pin initial structure
typedef struct {
    GPIO_InitTypeDef pin_init_struct;

    halGPIO_port port;          /**< enum value halGPIO_port */
    halGPIO_pinState state;     /**< enum value halGPIO_pinState.
                                         Used only for out type */
} halGPIO_struct;

/// initial parameters structure
typedef struct {
    halGPIO_pin pin;            /**< enum value halGPIO_pin */
    halGPIO_mode mode;          /**< enum value halGPIO_mode */
    halGPIO_speed speed;        /**< enum value halGPIO_speed */
    halGPIO_OType outType;      /**< enum value halGPIO_OType */
    halGPIO_PuPd PuPd;          /**< enum value halGPIO_PuPd */
} halGPIO_init_struct;

typedef struct {
  GPIO_TypeDef* const reg_port;
  uint32_t reg_port_clock;
} halGPIO_RegTable;

/**
 * Configured GPIO function.
 * @param gpioStruct pointer on structure that consist main configuration
 */
void halGPIOInit(halGPIO_struct* gpioStruct, const halGPIO_init_struct* initStruct);

/**
 * Set high level on gpio pin.
 * @param gpioStruct pointer on structure with data
 */
void halGPIOSetPins(halGPIO_struct* gpioStruct);

/**
 * Set low level on gpio pin.
 * @param gpioStruct pointer on structure with data
 */
void halGPIOResetPins(halGPIO_struct* gpioStruct);

/**
 * Toggle level on gpio pin.
 * @param gpioStruct pointer on structure with data
 */
void halGPIOTogglePins(halGPIO_struct* gpioStruct);

/**
 * Function that return current pin state.
 * @param gpioStruct pointer on structure with data
 * @return current pin state from halGPIO_pinState enumeration
 */
halGPIO_pinState halGPIOReadPin(halGPIO_struct* gpioStruct);

#endif /* HAL_HAL_GPIO_H_ */
