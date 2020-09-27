/*
 * hal_gpio.h
 *
 *  Created on: 29 ����. 2020 �.
 *  Author: vladyslav.daliavskyi
 */

#ifndef HAL_HAL_GPIO_H_
#define HAL_HAL_GPIO_H_

#include "stm32f4xx.h"

#define EXTI_PRIORITY     15
#define EXTI_SUBPRIORITY  16

/// Pin states
typedef enum {
  halGPIO_Set   = SET,  /**< Set state */
  halGPIO_Reset = RESET   /**< Reset state */
} halGPIO_pinState;

typedef struct {
  uint16_t pin;
  int8_t   exti;
} halGPIO_pinData_t;

/// Port's pin enumeration
typedef enum {
  halGPIO_Pin0,
  halGPIO_Pin1,
  halGPIO_Pin2,
  halGPIO_Pin3,
  halGPIO_Pin4,
  halGPIO_Pin5,
  halGPIO_Pin6,
  halGPIO_Pin7,
  halGPIO_Pin8,
  halGPIO_Pin9,
  halGPIO_Pin10,
  halGPIO_Pin11,
  halGPIO_Pin12,
  halGPIO_Pin13,
  halGPIO_Pin14,
  halGPIO_Pin15,
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
  uint32_t        enablePortClockDef;
} halGPIO_portDef_t;

/// Pins functional type enumeration
typedef enum {
  halGPIO_ModeIn,     /**< In type */
  halGPIO_ModeOut,    /**< Out type */
  halGPIO_ModeAF,     /**< Alternative function */
  halGPIO_ModeAn      /**< Analog read-write */
} halGPIO_mode;

/// Pins frequency enumeration
typedef enum {
  halGPIO_Speed2Mhz   = GPIO_SPEED_FREQ_LOW,     /**< 2 MHz */
  halGPIO_Speed25Mhz  = GPIO_SPEED_FREQ_MEDIUM,  /**< 25 MHz */
  halGPIO_Speed50Mhz  = GPIO_SPEED_FREQ_HIGH,    /**< 50 MHz */
  halGPIO_Speed100Mhz = GPIO_SPEED_FREQ_VERY_HIGH  /**< 100 MHz */
} halGPIO_speed;

/// Internal pull-up type enumeration
typedef enum {
  halGPIO_PuPdUp   = GPIO_PULLUP,     /**< Up to VCC. */
  halGPIO_PuPdDown = GPIO_PULLDOWN,     /**< Down to Ground. */
  halGPIO_PuPdNo   = GPIO_NOPULL,     /**< Not to ground, not to power. */
} halGPIO_PuPd;

/// Pin's out type enumeration
typedef enum {
  halGPIO_OTPP,  /**< Push-pull type. */
  halGPIO_OTOD   /**< Open-drain type. */
} halGPIO_OType;

/// GPIO pin initial structure
typedef struct {
  GPIO_InitTypeDef  pinInitStruct;

  halGPIO_port      port;
  halGPIO_pinState  state;   /**< Show current pint state.
                     Used only for out type */
} halGPIO_struct;

typedef void(*EXTIFunction)(uint16_t);

typedef enum {
  RISING = GPIO_MODE_IT_RISING,
  FALLING = GPIO_MODE_IT_FALLING,
  RISING_FALING = GPIO_MODE_IT_RISING_FALLING
} halGPIO_EXTItype;

typedef struct {
  EXTIFunction risingCallback;
  EXTIFunction fallingCallback;
  halGPIO_EXTItype type;
  GPIO_TypeDef* port;
} halGPIO_EXTIstruct;

/// initial parameters structure
typedef struct {
  halGPIO_pin         pin;      /**< Configured pin number. */
  halGPIO_mode        mode;     /**< Pin work mode. */
  halGPIO_speed       speed;    /**< Pin frequency */
  halGPIO_OType       outType;  /**< Output states */
  halGPIO_PuPd        PuPd;     /**< Push-pull direction */
  halGPIO_EXTIstruct* exti;
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
