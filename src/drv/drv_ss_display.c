/*
 * drv_ss_display.c
 *
 *  Created on: 3 лип. 2020 р.
 *      Author: vladyslav.daliavskyi
 */

#include "drv_ss_display.h"

// configuration data
drvSSDislay_struct display_data = {
    .anodes     =  {
        halGPIO_Pin3,       //a
        halGPIO_Pin4,       //b
        halGPIO_Pin5,       //c
        halGPIO_Pin6,       //d
        halGPIO_Pin7,       //e
        halGPIO_Pin8,       //f
        halGPIO_Pin9,       //g
        halGPIO_Pin10,      //dp
    },
    .cathodes   =  {
        halGPIO_Pin11,      // first number
        halGPIO_Pin12,      // second number
        halGPIO_Pin13       // third number
    },
    .port       = halGPIO_PortB,
};

// base configuration for anodes and cathodes
static halGPIO_initStruct gpioInitTemplateStruct = {
    .mode       = halGPIO_ModeOut,
    .speed      = halGPIO_Speed2Mhz,
    .outType    = halGPIO_OTPP,
    .PuPd       = halGPIO_PuPdNo
};

// table for DMA destination addresses
const static uint32_t dest_reg[halGPIO_PortCount] = {
    (uint32_t)&(GPIOA->ODR),
    (uint32_t)&(GPIOB->ODR),
    (uint32_t)&(GPIOC->ODR),
    (uint32_t)&(GPIOD->ODR),
    (uint32_t)&(GPIOE->ODR)
};

/*
 *  Symbols table
 *  Increment LIBRARY_CAPASITY value if add symbol to the table
 */

static drvSSDisplay_SymbolPresentation drvSSDisplay_symbolsLibrary[LIBRARY_CAPASITY] =
{
    {'0', seg_a| seg_b| seg_c| seg_d| seg_e| seg_f},
    {'1', seg_b| seg_c},
    {'2', seg_a| seg_b| seg_d| seg_e| seg_g},
    {'3', seg_a| seg_b| seg_c| seg_d| seg_g},
    {'4', seg_b| seg_c| seg_f| seg_g},
    {'5', seg_a| seg_c| seg_d| seg_f| seg_g},
    {'6', seg_a| seg_c| seg_d| seg_e| seg_f| seg_g},
    {'7', seg_a| seg_b| seg_c},
    {'8', seg_a| seg_b| seg_c| seg_d| seg_e| seg_f| seg_g},
    {'9', seg_a| seg_b| seg_c| seg_d| seg_f| seg_g},
    {' ', 0x00}
};

void drvSSDisplayInit() {
    halGPIO_struct gpioStruct = {
        .port   = display_data.port,
        .state  = halGPIO_Reset
    };

    // anodes configuratin
    for (uint8_t i = 0; i < DISPLAY_SEGMENT_COUNT; i++) {
        gpioInitTemplateStruct.pin = display_data.anodes[i];
        halGPIOInit(&gpioStruct, &gpioInitTemplateStruct);
    }

    gpioStruct.state = halGPIO_Set;

    // cathode configuration
    for (uint8_t i = 0; i < DISPLAY_NUMBER_COUNT; i++) {
        gpioInitTemplateStruct.pin = display_data.cathodes[i];
        halGPIOInit(&gpioStruct, &gpioInitTemplateStruct);
    }

    // setting anode bit in the array for DMA
    for (uint8_t i = 0; i < DISPLAY_NUMBER_COUNT; i++) {
        display_data.outValues[i] = 0xFF;
        for (uint8_t j = 0; j < DISPLAY_NUMBER_COUNT; j++) {
            (i != j) ?
                    (display_data.outValues[i] &= ~(display_data.cathodes[j])) :
                    (display_data.outValues[i] |= (display_data.cathodes[j]));
        }
    }

    // enable RCC for timer and DMA
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE);

    // timer configuration
    TIM_TimeBaseInitTypeDef timer;

    timer.TIM_ClockDivision = TIM_CKD_DIV4;
    timer.TIM_CounterMode = TIM_CounterMode_Up;
    timer.TIM_Period = TIMER_TICKS_PER_SECOND / DISPLAY_NUMBER_COUNT / DISPLAY_UPDATE_FREQ;
    timer.TIM_Prescaler = TIMER_CLOCK_PRESCULAR;
    timer.TIM_RepetitionCounter = 0x00;

    TIM_TimeBaseInit(TIM1, &timer);
    TIM_ITConfig(TIM1, TIM_IT_Update, ENABLE);
    TIM_ClearITPendingBit(TIM1, TIM_IT_Update);

    // Connect TIM_UP event to DMA
    TIM_DMACmd(TIM1, TIM_DMA_Update, ENABLE);

    // DMA configuration
    DMA_InitTypeDef dma;

    dma.DMA_BufferSize = sizeof(display_data.outValues)/sizeof(display_data.outValues[0]);
    dma.DMA_Channel = DMA_Channel_6;
    dma.DMA_DIR = DMA_DIR_MemoryToPeripheral;
    dma.DMA_Memory0BaseAddr = (uint32_t) display_data.outValues;
    dma.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
    dma.DMA_MemoryInc = DMA_MemoryInc_Enable;
    dma.DMA_Mode = DMA_Mode_Circular;
    dma.DMA_PeripheralBaseAddr = dest_reg[display_data.port];
    dma.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
    dma.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    dma.DMA_Priority = DMA_Priority_High;

    dma.DMA_FIFOMode = DMA_FIFOMode_Disable;
    dma.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;
    dma.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
    dma.DMA_MemoryBurst = DMA_MemoryBurst_Single;

    DMA_Init(DMA2_Stream5, &dma);

    DMA_ITConfig(DMA2_Stream5, DMA_IT_TC, ENABLE);

    // start DMA
    DMA_Cmd(DMA2_Stream5, ENABLE);

    // start timer
    TIM_Cmd(TIM1, ENABLE);
}


// Function change element in DMA array in case to character
static uint8_t drvSSDisplayGetSymbolDefinition(char character) {
    for (uint8_t i = 0; i < LIBRARY_CAPASITY; i++) {
        if(drvSSDisplay_symbolsLibrary[i].character == character) {
            return drvSSDisplay_symbolsLibrary[i].symbol_def;
        }
    }
    return 0x00;
}

// Function change element in DMA array in case to symbol definition
static void drvSSDispalySetSymbol(uint8_t pos, uint8_t symbolDef) {
    for(uint8_t i=0; i<DISPLAY_SEGMENT_COUNT; i++) {
        ((symbolDef >> i) & 0x01) != 0x01 ?
                (display_data.outValues[pos] |= display_data.anodes[i]) :
                (display_data.outValues[pos] &= ~(display_data.anodes[i]));
    }
}

// Function change display number text
void drvSSDisplaySetValue(uint8_t value) {
    for (uint8_t i = 0; i < DISPLAY_NUMBER_COUNT; i++) {
        drvSSDispalySetSymbol(i, drvSSDisplayGetSymbolDefinition((value % 10) + 48));
        value /= 10;
    }
}
