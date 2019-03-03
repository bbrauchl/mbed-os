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

#include "PeripheralPins.h"

/************RTC***************/
const PinMap PinMap_RTC[] = {
    {NC, OSC32KCLK, 0},
};

/************ADC***************/
const PinMap PinMap_ADC[] = {
    {PTC3,  ADC0_SE11, 0},
    {PTC2,  ADC0_SE10, 0},
    {PTC1,  ADC0_SE9,  0},
    {PTC0,  ADC0_SE8,  0},
    {PTC17, ADC0_SE15, 0},
    {PTC16, ADC0_SE14, 0},
    {PTC15, ADC0_SE13, 0},
    {PTC14, ADC0_SE12, 0},
    {PTB3,  ADC0_SE7,  0},
    {PTB2,  ADC0_SE6,  0},
    {PTB1,  ADC0_SE5,  0},
    {PTB0,  ADC0_SE4,  0},
    {PTA7,  ADC0_SE3,  0},
    {PTA6,  ADC0_SE2,  0},
    {PTB14, ADC1_SE9,  0},
    {PTB13, ADC1_SE8,  0},
    {PTB12, ADC1_SE7,  0},
    {PTD4,  ADC1_SE6,  0},
    {PTD3,  ADC1_SE3,  0},
    {PTD2,  ADC1_SE2,  0},
    {PTA3,  ADC1_SE1,  0},
    {PTA2,  ADC1_SE0,  0},
    {PTA1,  ADC0_SE1,  0},
    {PTA0,  ADC0_SE0,  0},
    {PTC7,  ADC1_SE5,  0},
    {PTC6,  ADC1_SE4,  0},
    {PTE6,  ADC1_SE11, 0},
    {PTE2,  ADC1_SE10, 0},
    {NC,    NC,        0}
};

/************I2C***************/
const PinMap PinMap_I2C_SDA[] = {
    {PTB6,  LPI2C_0, 2},
    {PTD8,  LPI2C_1, 2},
    {PTA2,  LPI2C_0, 3},
    {PTE0,  LPI2C_1, 4},
    {NC,    NC,      0}
};

const PinMap PinMap_I2C_SCL[] = {
    {PTB7,  LPI2C_0, 2},
    {PTD9,  LPI2C_1, 2},
    {PTA3,  LPI2C_0, 3},
    {PTE1,  LPI2C_1, 4},
    {NC,    NC,      0}
};

/************LPUART***************/
const PinMap PinMap_UART_TX[] = {
    {PTE12, LPUART_2, 3},
    {PTD7,  LPUART_2, 2},
    {PTB1,  LPUART_0, 2},
    {PTC9,  LPUART_1, 2},
    {PTA3,  LPUART_0, 6},
    {PTC7,  LPUART_1, 2},
    {PTA10, LPUART_0, 3},
    {NC,    NC,       0}
};

const PinMap PinMap_UART_RX[] = {
    {PTD17, LPUART_2, 3},
    {PTD6,  LPUART_2, 2},
    {PTB0,  LPUART_0, 2},
    {PTC8,  LPUART_1, 2},
    {PTA2,  LPUART_0, 6},
    {PTC6,  LPUART_1, 2},
    {PTA11, LPUART_0, 3},
    {NC,    NC,       0}
};

/************SPI***************/
const PinMap PinMap_SPI_SCLK[] = {
    {PTD0,  LPSPI1_PCS0, 3},
    {PTB2,  LPSPI0_PCS0, 3},
    {PTB14, LPSPI1_PCS0, 3},
    {PTE0,  LPSPI0_PCS0, 2},
    {NC,    NC,      0}
};

/* Using pins labeled LPUARTx_Sout */
const PinMap PinMap_SPI_MOSI[] = {
    {PTB4,  LPSPI0_PCS0, 3},
    {PTB1,  LPSPI0_PCS0, 3},
    {PTB16, LPSPI1_PCS0, 3},
    {PTD2,  LPSPI1_PCS0, 3},
    {PTE2,  LPSPI0_PCS0, 2},
    {NC,    NC,      0}
};

/* Using pins labeled LPUARTx_SIN */
const PinMap PinMap_SPI_MISO[] = {
    {PTD1,  LPSPI1_PCS0, 3},
    {PTB3,  LPSPI0_PCS0, 3},
    {PTB15, LPSPI1_PCS0, 3},
    {PTE1,  LPSPI0_PCS0, 2},
    {NC,    NC,      0}
};

const PinMap PinMap_SPI_SSEL[] = {
    {PTB0,  LPSPI0_PCS0, 3}, //PCS0
    {PTD3,  LPSPI1_PCS0, 3}, //PCS0
    {PTB5,  LPSPI0_PCS1, 3}, //PCS1
    {PTA6,  LPSPI1_PCS1, 3}, //PCS1
    {PTE6,  LPSPI0_PCS2, 2}, //PCS2
    {PTA16, LPSPI1_PCS2, 3}, //PCS2
    {PTA15, LPSPI0_PCS3, 3}, //PCS3
    {PTB17, LPSPI1_PCS3, 3}, //PCS3
    {NC,   NC,      0}
};

/************PWM***************/
const PinMap PinMap_PWM[] = {
    {PTD1,  PWM_4,  2},
    //{PTD1,  PWM_14, 4},
    {PTD0,  PWM_3,  2},
    //{PTD0,  PWM_13, 4},
    {PTE5,  PWM_16, 4},
    {PTE4,  PWM_15, 4},
    {PTD16, PWM_2,  2},
    {PTD15, PWM_1,  2},
    {PTE9,  PWM_8,  2},
    {PTE8,  PWM_7,  2},
    {PTB5,  PWM_6,  2},
    {PTB4,  PWM_5,  2},
    {PTC3,  PWM_4,  2},
    {PTC2,  PWM_3,  2},
    {PTD5,  PWM_16, 2},
    {PTD12, PWM_15, 2},
    {PTD11, PWM_14, 2},
    {PTD10, PWM_13, 2},
    {PTC1,  PWM_2,  2},
    {PTC0,  PWM_1,  2},
    {PTC15, PWM_12, 2},
    {PTC14, PWM_11, 2},
    {PTB3,  PWM_10, 2},
    {PTB2,  PWM_9,  2},
    {PTE7,  PWM_8,  2},
    {PTA17, PWM_7,  2},
    {PTB17, PWM_6,  2},
    {PTB16, PWM_5,  2},
    {PTB15, PWM_4,  2},
    {PTB14, PWM_3,  2},
    {PTB13, PWM_2,  2},
    {PTB12, PWM_1,  2},
    {PTA1,  PWM_10, 2},
    {PTA0,  PWM_14, 2},
    {PTA16, PWM_12, 2},
    {PTA15, PWM_11, 2},
    {PTC5,  PWM_13, 2},
    {PTC4,  PWM_9,  2},
    {NC   , NC    , 0}
};

