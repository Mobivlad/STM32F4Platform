/*
 * hal_gpio.h
 *
 *  Created on: 29 трав. 2020 р.
 *      Author: vladyslav.daliavskyi
 */

#ifndef HAL_HAL_GPIO_H_
#define HAL_HAL_GPIO_H_

#include "stm32f4xx.h"

/// Port's pin enumeration
typedef enum {
    halPin_0  = GPIO_Pin_0,      /**< Pin 0 */
    halPin_1  = GPIO_Pin_1,      /**< Pin 1 */
    halPin_2  = GPIO_Pin_2,      /**< Pin 2 */
    halPin_3  = GPIO_Pin_3,      /**< Pin 3 */
    halPin_4  = GPIO_Pin_4,      /**< Pin 4 */
    halPin_5  = GPIO_Pin_5,      /**< Pin 5 */
    halPin_6  = GPIO_Pin_6,      /**< Pin 6 */
    halPin_7  = GPIO_Pin_7,      /**< Pin 7 */
    halPin_8  = GPIO_Pin_8,      /**< Pin 8 */
    halPin_9  = GPIO_Pin_9,      /**< Pin 9 */
    halPin_10 = GPIO_Pin_10,     /**< Pin 10 */
    halPin_11 = GPIO_Pin_11,     /**< Pin 11 */
    halPin_12 = GPIO_Pin_12,     /**< Pin 12 */
    halPin_13 = GPIO_Pin_13,     /**< Pin 13 */
    halPin_14 = GPIO_Pin_14,     /**< Pin 14 */
    halPin_15 = GPIO_Pin_15      /**< Pin 15 */
} halPins;

/// STM32F407-Disk1 port enumeration
typedef enum {
    halPortA,   /**< Port A */
    halPortB,   /**< Port B */
    halPortC,   /**< Port C */
    halPortD,   /**< Port D */
    halPortE,   /**< Port E */
} halPort;

/// STM32F407-Disk1 RCC port values enumeration
typedef enum {
    halRCCPortA = RCC_AHB1Periph_GPIOA,   /**< Port A */
    halRCCPortB = RCC_AHB1Periph_GPIOB,   /**< Port B */
    halRCCPortC = RCC_AHB1Periph_GPIOC,   /**< Port C */
    halRCCPortD = RCC_AHB1Periph_GPIOD,   /**< Port D */
    halRCCPortE = RCC_AHB1Periph_GPIOE   /**< Port E */
} halRCCvalues;

/// Pins functional type enumeration
typedef enum {
    halModeIn  = GPIO_Mode_IN,      /**< In type */
    halModeOut = GPIO_Mode_OUT,     /**< Out type */
    halModeAF  = GPIO_Mode_AF,      /**< Alternative function */
    halModeAn  = GPIO_Mode_AN       /**< Analog read-write */
} halGpioMode;

/// Pins frequency enumeration
typedef enum {
    halSpeed2Mhz   = GPIO_Low_Speed,        /**< 2 MHz */
    halSpeed25Mhz  = GPIO_Medium_Speed,     /**< 25 MHz */
    halSpeed50Mhz  = GPIO_High_Speed,       /**< 50 MHz */
    halSpeed100Mhz = GPIO_Fast_Speed        /**< 100 MHz */
} halGpioSpeed;

/// Internal pull-up type enumeration
typedef enum {
    halPuPdUp   = GPIO_PuPd_UP,     /**< Up to VCC. */
    halPuPdDown = GPIO_PuPd_DOWN,   /**< Down to Ground. */
    halPuPdNo   = GPIO_PuPd_NOPULL, /**< Not to ground, not to power. */
} halPuPd;

/// Pin's out type enumeration
typedef enum {
    halOTPP = GPIO_OType_PP,        /**< Push-pull type. */
    halOTOD = GPIO_OType_OD         /**< Open-drain type. */
} halOutType;

/// Pin's logic level enumeration
typedef enum {
    halPinStateSet   = Bit_SET,     /**< high level. */
    halPinStateReset = Bit_RESET    /**< low level. */
} halPinState;

/// GPIO pin initial structure
typedef struct {
    halPins halPins;            /**< enum value halPins */
    halGpioMode halGpioMode;    /**< enum value halGpioMode */
    halGpioSpeed halGpioSpeed;  /**< enum value halGpioSpeed */
    halOutType halOutType;      /**< enum value halOutType */
    halPuPd halPuPd;            /**< enum value halPuPd */
} halInitGPIOStruct;

/**
 * Function that configurate gpio pins.
 * @param halPort value from halPort enumeration
 * @param initStruct pointer to structure that consist of main port configuration
 */
void halGPIOInit(halPort halPort,halInitGPIOStruct* initStruct);

/**
 * Function to set high level on gpio pin.
 * @param port value of halPort enumeration
 * @param pins value of halPin enumeration
 */
void halSetBits(halPort port,halPins pins);

/**
 * Function to set low level on gpio pin.
 * @param port value of halPort enumeration
 * @param pins value of halPin enumeration
 */
void halResetBits(halPort port,halPins pins);

/**
 * Function to change logic level on gpio pin.
 * @param port value of halPort enumeration
 * @param pins value of halPin enumeration
 */
void halToggle(halPort port,halPins pins);

/**
 * Function that return current pin state.
 * @param port value of halPort enumeration
 * @param pins value of halPin enumeration
 * @return current pin state from halPinState enumeration
 */
halPinState halReadPin(halPort port,halPins pin);

#endif /* HAL_HAL_GPIO_H_ */
