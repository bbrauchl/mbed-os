/* mbed Microcontroller Library
 * Copyright (c) 2006-2013 ARM Limited
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#ifndef MBED_PERIPHERALNAMES_H
#define MBED_PERIPHERALNAMES_H

#include "cmsis.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    OSC32KCLK = 0,
} RTCName;

/* LPUART */
typedef enum {
    LPUART_0 = 0,
    LPUART_1 = 1,
    LPUART_2 = 2,
} UARTName;

#define STDIO_UART_TX     USBTX
#define STDIO_UART_RX     USBRX
#define STDIO_UART        LPUART_1

typedef enum {
    LPI2C_0 = 0,
    LPI2C_1 = 1,
} I2CName;

#define FTM_SHIFT   8
typedef enum {
    PWM_1  = (0 << FTM_SHIFT) | (0),  // FTM0 CH0
    PWM_2  = (0 << FTM_SHIFT) | (1),  // FTM0 CH1
    PWM_3  = (0 << FTM_SHIFT) | (2),  // FTM0 CH2
    PWM_4  = (0 << FTM_SHIFT) | (3),  // FTM0 CH3
    PWM_5  = (0 << FTM_SHIFT) | (4),  // FTM0 CH4
    PWM_6  = (0 << FTM_SHIFT) | (5),  // FTM0 CH5
    PWM_7  = (0 << FTM_SHIFT) | (6),  // FTM0 CH6
    PWM_8  = (0 << FTM_SHIFT) | (7),  // FTM0 CH7
    PWM_9  = (1 << FTM_SHIFT) | (0),  // FTM1 CH0
    PWM_10 = (1 << FTM_SHIFT) | (1),  // FTM1 CH1
    PWM_11 = (1 << FTM_SHIFT) | (2),  // FTM1 CH2
    PWM_12 = (1 << FTM_SHIFT) | (3),  // FTM1 CH3
    PWM_13 = (2 << FTM_SHIFT) | (0),  // FTM2 CH0
    PWM_14 = (2 << FTM_SHIFT) | (1),  // FTM2 CH1
    PWM_15 = (2 << FTM_SHIFT) | (2),  // FTM2 CH2
    PWM_16 = (2 << FTM_SHIFT) | (3),  // FTM2 CH3
} PWMName;

#define ADC_INSTANCE_SHIFT           8
#define ADC_B_CHANNEL_SHIFT          5
typedef enum {
    ADC0_SE0  = (0 << ADC_INSTANCE_SHIFT) | 0,
    ADC0_SE1  = (0 << ADC_INSTANCE_SHIFT) | 1,
    ADC0_SE2  = (0 << ADC_INSTANCE_SHIFT) | 2,
    ADC0_SE3  = (0 << ADC_INSTANCE_SHIFT) | 3,
    ADC0_SE4  = (0 << ADC_INSTANCE_SHIFT) | 4,
    ADC0_SE5  = (0 << ADC_INSTANCE_SHIFT) | 5,
    ADC0_SE6  = (0 << ADC_INSTANCE_SHIFT) | 6,
    ADC0_SE7  = (0 << ADC_INSTANCE_SHIFT) | 7,
    ADC0_SE8  = (0 << ADC_INSTANCE_SHIFT) | 8,
    ADC0_SE9  = (0 << ADC_INSTANCE_SHIFT) | 9,
    ADC0_SE10  = (0 << ADC_INSTANCE_SHIFT) | 10,
    ADC0_SE11 = (0 << ADC_INSTANCE_SHIFT) | 11,
    ADC0_SE12 = (0 << ADC_INSTANCE_SHIFT) | 12,
    ADC0_SE13 = (0 << ADC_INSTANCE_SHIFT) | 13,
    ADC0_SE14 = (0 << ADC_INSTANCE_SHIFT) | 14,
    ADC0_SE15 = (0 << ADC_INSTANCE_SHIFT) | 15,
    ADC1_SE0  = (1 << ADC_INSTANCE_SHIFT) | 0,
    ADC1_SE1  = (1 << ADC_INSTANCE_SHIFT) | 1,
    ADC1_SE2  = (1 << ADC_INSTANCE_SHIFT) | 2,
    ADC1_SE3  = (1 << ADC_INSTANCE_SHIFT) | 3,
    ADC1_SE4  = (1 << ADC_INSTANCE_SHIFT) | 4,
    ADC1_SE5  = (1 << ADC_INSTANCE_SHIFT) | 5,
    ADC1_SE6  = (1 << ADC_INSTANCE_SHIFT) | 6,
    ADC1_SE7  = (1 << ADC_INSTANCE_SHIFT) | 7,
    ADC1_SE8  = (1 << ADC_INSTANCE_SHIFT) | 8,
    ADC1_SE9  = (1 << ADC_INSTANCE_SHIFT) | 9,
    ADC1_SE10 = (1 << ADC_INSTANCE_SHIFT) | 10,
    ADC1_SE11 = (1 << ADC_INSTANCE_SHIFT) | 11,
} ADCName;

typedef enum {
    DAC_0 = 0
} DACName;

#define LPSPI_INSTANCE_SHIFT		8
#define LPSPI_PCS_SHIFT				4
typedef enum {
    LPSPI0_PCS0 = (0 << LPSPI_INSTANCE_SHIFT) | (0 << LPSPI_PCS_SHIFT),
    LPSPI0_PCS1 = (0 << LPSPI_INSTANCE_SHIFT) | (1 << LPSPI_PCS_SHIFT),
    LPSPI0_PCS2 = (0 << LPSPI_INSTANCE_SHIFT) | (2 << LPSPI_PCS_SHIFT),
    LPSPI0_PCS3 = (0 << LPSPI_INSTANCE_SHIFT) | (3 << LPSPI_PCS_SHIFT),
    LPSPI1_PCS0 = (1 << LPSPI_INSTANCE_SHIFT) | (0 << LPSPI_PCS_SHIFT),
    LPSPI1_PCS1 = (1 << LPSPI_INSTANCE_SHIFT) | (1 << LPSPI_PCS_SHIFT),
    LPSPI1_PCS2 = (1 << LPSPI_INSTANCE_SHIFT) | (2 << LPSPI_PCS_SHIFT),
    LPSPI1_PCS3 = (1 << LPSPI_INSTANCE_SHIFT) | (3 << LPSPI_PCS_SHIFT),
} SPIName;

#ifdef __cplusplus
}
#endif

#endif
