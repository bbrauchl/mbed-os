/* mbed Microcontroller Library
 * Copyright (c) 2006-2018 ARM Limited
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
#include <stddef.h>
#include "us_ticker_api.h"
#include "PeripheralNames.h"
#include "fsl_lpit.h"
#include "clock_config.h"

const ticker_info_t* us_ticker_get_info()
{
    static const ticker_info_t info = {
        1000000,    // 1 MHz
             32     // 32 bit counter
    };
    return &info;
}

static bool us_ticker_inited = false;

static void lpit0_isr(void)
{
    LPIT_ClearStatusFlags(LPIT0, LPIT_MSR_TIF3_MASK);
    LPIT_ClearStatusFlags(LPIT0, LPIT_MSR_TIF2_MASK);
    LPIT_StopTimer(LPIT0, kLPIT_Chnl_2);
    LPIT_StopTimer(LPIT0, kLPIT_Chnl_3);

    us_ticker_irq_handler();
}

/** Initialize the high frequency ticker
 *
 */
void us_ticker_init(void)
{
    /* Common for ticker/timer. */
    uint32_t lpitClockFreq;
    /* Structure to initialize LPIT. */
    lpit_config_t lpitConfig;
    /* Structure to initalize unchained channels of LPIT */
    lpit_chnl_params_t lpitChannelConfig = {
                .chainChannel = false,
                .timerMode = kLPIT_PeriodicCounter,
                .triggerSelect = kLPIT_Trigger_TimerChn0,
                .triggerSource = kLPIT_TriggerSource_External,
                .enableReloadOnTrigger = false,
                .enableStopOnTimeout = false,
                .enableStartOnTrigger = false,
    };
    /* Structure to initalize chained channels of LPIT */
    lpit_chnl_params_t lpitChannelConfigChainMode = {
                .chainChannel = true,
                .timerMode = kLPIT_PeriodicCounter,
                .triggerSelect = kLPIT_Trigger_TimerChn0,
                .triggerSource = kLPIT_TriggerSource_External,
                .enableReloadOnTrigger = false,
                .enableStopOnTimeout = false,
                .enableStartOnTrigger = false,
    };

    LPIT_GetDefaultConfig(&lpitConfig);
    lpitConfig.enableRunInDoze = true; //The lpit must be enabled in doze because the os puts the part in low power mode when idle.
    LPIT_Init(LPIT0, &lpitConfig);
    CLOCK_SetIpSrc(kCLOCK_Lpit0, kCLOCK_IpSrcFircAsync);
    lpitClockFreq = CLOCK_GetIpFreq(kCLOCK_Lpit0);

    /* Let the timer to count if re-init. */
    if (!us_ticker_inited) {

        LPIT_SetTimerPeriod(LPIT0, kLPIT_Chnl_0, lpitClockFreq / 1000000 - 1);
        LPIT_SetTimerPeriod(LPIT0, kLPIT_Chnl_1, 0xFFFFFFFF);
        LPIT_SetupChannel(LPIT0, kLPIT_Chnl_0, &lpitChannelConfig);
        LPIT_SetupChannel(LPIT0, kLPIT_Chnl_1, &lpitChannelConfigChainMode);
        LPIT_StartTimer(LPIT0, kLPIT_Chnl_0);
        LPIT_StartTimer(LPIT0, kLPIT_Chnl_1);
    }

    /* Configure interrupt generation counters and disable ticker interrupts. */
    LPIT_StopTimer(LPIT0, kLPIT_Chnl_3);
    LPIT_StopTimer(LPIT0, kLPIT_Chnl_2);
    LPIT_SetTimerPeriod(LPIT0, kLPIT_Chnl_2, lpitClockFreq / 1000000 - 1);
    LPIT_SetupChannel(LPIT0, kLPIT_Chnl_2, &lpitChannelConfig);
    LPIT_SetupChannel(LPIT0, kLPIT_Chnl_3, &lpitChannelConfigChainMode);
    NVIC_SetVector(LPIT0_IRQn, (uint32_t) lpit0_isr);
    NVIC_EnableIRQ(LPIT0_IRQn);
    LPIT_DisableInterrupts(LPIT0, LPIT_MIER_TIE3_MASK);

    us_ticker_inited = true;
}

/** Read the current counter
 *
 * @return The current timer's counter value in ticks
 */
uint32_t us_ticker_read()
{
    return ~(LPIT_GetCurrentTimerCount(LPIT0, kLPIT_Chnl_1));
}

/** Disable us ticker interrupt
 *
 */
void us_ticker_disable_interrupt(void)
{
    LPIT_DisableInterrupts(LPIT0, LPIT_MIER_TIE3_MASK);
}

/** Clear us ticker interrupt
 *
 */
void us_ticker_clear_interrupt(void)
{
    LPIT_ClearStatusFlags(LPIT0, LPIT_MSR_TIF3_MASK);
}

/** Set interrupt for specified timestamp
 *
 * @param timestamp The time in ticks when interrupt should be generated
 */
void us_ticker_set_interrupt(timestamp_t timestamp)
{
    /* We get here absolute interrupt time which takes into account counter overflow.
     * Since we use additional count-down timer to generate interrupt we need to calculate
     * load value based on time-stamp.
     */
    const uint32_t now_ticks = us_ticker_read();
    uint32_t delta_ticks =
            timestamp >= now_ticks ? timestamp - now_ticks : (uint32_t)((uint64_t) timestamp + 0xFFFFFFFF - now_ticks);

    if (delta_ticks == 0) {
        /* The requested delay is less than the minimum resolution of this counter. */
        delta_ticks = 1;
    }

    LPIT_StopTimer(LPIT0, kLPIT_Chnl_3);
    LPIT_StopTimer(LPIT0, kLPIT_Chnl_2);
    LPIT_SetTimerPeriod(LPIT0, kLPIT_Chnl_3, delta_ticks);
    LPIT_EnableInterrupts(LPIT0, LPIT_MIER_TIE3_MASK);
    LPIT_StartTimer(LPIT0, kLPIT_Chnl_3);
    LPIT_StartTimer(LPIT0, kLPIT_Chnl_2);
}

/** Fire us ticker interrupt
 *
 */
void us_ticker_fire_interrupt(void)
{
    NVIC_SetPendingIRQ(LPIT0_IRQn);
}

void us_ticker_free(void)
{

}

