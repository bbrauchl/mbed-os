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

#ifndef ANALOGIN_API_C
#define ANALOGIN_API_C

#include "mbed_assert.h"
#include "analogin_api.h"

#if DEVICE_ANALOGIN

#include "cmsis.h"
#include "pinmap.h"
#include "PeripheralNames.h"
#include "fsl_adc12.h"
#include "PeripheralPins.h"

/* Array of ADC peripheral base address. */
static ADC_Type *const adc_addrs[] = ADC_BASE_PTRS;
static clock_ip_name_t const adc_clocks[] = ADC12_CLOCKS;

#define MAX_FADC 6000000

void analogin_init(analogin_t *obj, PinName pin)
{
    obj->adc = (ADCName)pinmap_peripheral(pin, PinMap_ADC);
    MBED_ASSERT(obj->adc != (ADCName)NC);

    uint32_t instance = obj->adc >> ADC_INSTANCE_SHIFT;
    uint32_t adcClock;
    adc12_config_t adc12_config;

    CLOCK_SetIpSrc(adc_clocks[instance], kCLOCK_IpSrcLpFllAsync);
    adcClock = CLOCK_GetIpFreq(adc_clocks[instance]);
    uint32_t clkdiv;
    for (clkdiv = 0; clkdiv < 4; clkdiv++) {
        if ((adcClock >> clkdiv) <= MAX_FADC)
            break;
    }
    if (clkdiv == 4) {
        clkdiv = 0x3; //Set max div
    }

    ADC12_GetDefaultConfig(&adc12_config);
    adc12_config.clockSource = kADC12_ClockSourceAlt0;
    adc12_config.clockDivider = (adc12_clock_divider_t)clkdiv;
    adc12_config.resolution = kADC12_Resolution12Bit;
    ADC12_Init(adc_addrs[instance], &adc12_config);
    ADC12_EnableHardwareTrigger(adc_addrs[instance], false);
    ADC12_SetHardwareAverage(adc_addrs[instance], kADC12_HardwareAverageCount4);
    pinmap_pinout(pin, PinMap_ADC);
}

uint16_t analogin_read_u12(analogin_t *obj)
{
    uint32_t instance = obj->adc >> ADC_INSTANCE_SHIFT;
    adc12_channel_config_t adc12_channel_config;

    adc12_channel_config.channelNumber = obj->adc & 0xF;
    adc12_channel_config.enableInterruptOnConversionCompleted = false;

    /*
     * When in software trigger mode, each conversion would be launched once calling the "ADC12_ChannelConfigure()"
     * function, which works like writing a conversion command and executing it.
     */
    ADC12_SetChannelConfig(adc_addrs[instance], 0, &adc12_channel_config);
    while (0U == (kADC12_ChannelConversionCompletedFlag &
                      ADC12_GetChannelStatusFlags(adc_addrs[instance], 0)))
    {
    }
    return ADC12_GetChannelConversionValue(adc_addrs[instance], 0);
}

float analogin_read(analogin_t *obj)
{
    uint16_t value = analogin_read_u12(obj);
    return (float)value * (1.0f / (float)0xFFF);
}

#endif
#endif /* ANALOGIN_API_C */