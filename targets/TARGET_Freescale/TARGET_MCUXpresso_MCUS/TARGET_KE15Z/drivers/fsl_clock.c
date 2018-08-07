/*
 * The Clear BSD License
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * Copyright (c) 2016 - 2017 , NXP
 * All rights reserved.
 *
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted (subject to the limitations in the disclaimer below) provided
 * that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of copyright holder nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * NO EXPRESS OR IMPLIED LICENSES TO ANY PARTY'S PATENT RIGHTS ARE GRANTED BY THIS LICENSE.
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "fsl_clock.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

#define SCG_SIRC_LOW_RANGE_FREQ 2000000U  /* Slow IRC low range clock frequency. */
#define SCG_SIRC_HIGH_RANGE_FREQ 8000000U /* Slow IRC high range clock frequency.   */

#define SCG_FIRC_FREQ0 48000000U /* Fast IRC trimed clock frequency(48MHz). */
#define SCG_FIRC_FREQ1 52000000U /* Fast IRC trimed clock frequency(52MHz). */
#define SCG_FIRC_FREQ2 56000000U /* Fast IRC trimed clock frequency(56MHz). */
#define SCG_FIRC_FREQ3 60000000U /* Fast IRC trimed clock frequency(60MHz). */

#define SCG_LPFLL_FREQ0 48000000U  /* LPFLL trimed clock frequency(48MHz). */
#define SCG_LPFLL_FREQ1 72000000U  /* LPFLL trimed clock frequency(72MHz). */
#define SCG_LPFLL_FREQ2 96000000U  /* LPFLL trimed clock frequency(96MHz). */
#define SCG_LPFLL_FREQ3 120000000U /* LPFLL trimed clock frequency(120MHz). */

#define SCG_CSR_SCS_VAL ((SCG->CSR & SCG_CSR_SCS_MASK) >> SCG_CSR_SCS_SHIFT)
#define SCG_SOSCDIV_SOSCDIV1_VAL ((SCG->SOSCDIV & SCG_SOSCDIV_SOSCDIV1_MASK) >> SCG_SOSCDIV_SOSCDIV1_SHIFT)
#define SCG_SOSCDIV_SOSCDIV2_VAL ((SCG->SOSCDIV & SCG_SOSCDIV_SOSCDIV2_MASK) >> SCG_SOSCDIV_SOSCDIV2_SHIFT)
#define SCG_SIRCDIV_SIRCDIV1_VAL ((SCG->SIRCDIV & SCG_SIRCDIV_SIRCDIV1_MASK) >> SCG_SIRCDIV_SIRCDIV1_SHIFT)
#define SCG_SIRCDIV_SIRCDIV2_VAL ((SCG->SIRCDIV & SCG_SIRCDIV_SIRCDIV2_MASK) >> SCG_SIRCDIV_SIRCDIV2_SHIFT)
#define SCG_FIRCDIV_FIRCDIV1_VAL ((SCG->FIRCDIV & SCG_FIRCDIV_FIRCDIV1_MASK) >> SCG_FIRCDIV_FIRCDIV1_SHIFT)
#define SCG_FIRCDIV_FIRCDIV2_VAL ((SCG->FIRCDIV & SCG_FIRCDIV_FIRCDIV2_MASK) >> SCG_FIRCDIV_FIRCDIV2_SHIFT)

#define SCG_LPFLLDIV_LPFLLDIV1_VAL ((SCG->LPFLLDIV & SCG_LPFLLDIV_LPFLLDIV1_MASK) >> SCG_LPFLLDIV_LPFLLDIV1_SHIFT)
#define SCG_LPFLLDIV_LPFLLDIV2_VAL ((SCG->LPFLLDIV & SCG_LPFLLDIV_LPFLLDIV2_MASK) >> SCG_LPFLLDIV_LPFLLDIV2_SHIFT)

#define SCG_SIRCCFG_RANGE_VAL ((SCG->SIRCCFG & SCG_SIRCCFG_RANGE_MASK) >> SCG_SIRCCFG_RANGE_SHIFT)
#define SCG_FIRCCFG_RANGE_VAL ((SCG->FIRCCFG & SCG_FIRCCFG_RANGE_MASK) >> SCG_FIRCCFG_RANGE_SHIFT)

#define SCG_LPFLLCFG_FSEL_VAL ((SCG->LPFLLCFG & SCG_LPFLLCFG_FSEL_MASK) >> SCG_LPFLLCFG_FSEL_SHIFT)

#define PCC_PCS_VAL(reg) ((reg & PCC_CLKCFG_PCS_MASK) >> PCC_CLKCFG_PCS_SHIFT)

/*******************************************************************************
 * Variables
 ******************************************************************************/

/* External XTAL0 (OSC0) clock frequency. */
uint32_t g_xtal0Freq;
/* External XTAL32K clock frequency. */
uint32_t g_xtal32Freq;

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Code
 ******************************************************************************/

uint32_t CLOCK_GetErClkFreq(void)
{
    if (SCG->SOSCCSR & SCG_SOSCCSR_SOSCEN_MASK)
    {
        /* Please call CLOCK_SetXtal0Freq base on board setting before using OSC0 clock. */
        assert(g_xtal0Freq);
        return g_xtal0Freq;
    }
    else
    {
        return 0U;
    }
}

uint32_t CLOCK_GetOsc32kClkFreq(void)
{
    assert(g_xtal32Freq);
    return g_xtal32Freq;
}

uint32_t CLOCK_GetFlashClkFreq(void)
{
    return CLOCK_GetSysClkFreq(kSCG_SysClkSlow);
}

uint32_t CLOCK_GetBusClkFreq(void)
{
    return CLOCK_GetSysClkFreq(kSCG_SysClkSlow);
}

uint32_t CLOCK_GetCoreSysClkFreq(void)
{
    return CLOCK_GetSysClkFreq(kSCG_SysClkCore);
}

uint32_t CLOCK_GetFreq(clock_name_t clockName)
{
    uint32_t freq;

    switch (clockName)
    {
        case kCLOCK_CoreSysClk:
            freq = CLOCK_GetSysClkFreq(kSCG_SysClkCore);
            break;
        case kCLOCK_BusClk:
        case kCLOCK_FlashClk:
            freq = CLOCK_GetSysClkFreq(kSCG_SysClkSlow);
            break;

        case kCLOCK_ScgSysOscClk:
            freq = CLOCK_GetSysOscFreq();
            break;
        case kCLOCK_ScgSircClk:
            freq = CLOCK_GetSircFreq();
            break;
        case kCLOCK_ScgFircClk:
            freq = CLOCK_GetFircFreq();
            break;
        case kCLOCK_ScgLpFllClk:
            freq = CLOCK_GetLpFllFreq();
            break;

        case kCLOCK_ScgSysOscAsyncDiv2Clk:
            freq = CLOCK_GetSysOscAsyncFreq(kSCG_AsyncDiv2Clk);
            break;

        case kCLOCK_ScgSircAsyncDiv2Clk:
            freq = CLOCK_GetSircAsyncFreq(kSCG_AsyncDiv2Clk);
            break;

        case kCLOCK_ScgFircAsyncDiv2Clk:
            freq = CLOCK_GetFircAsyncFreq(kSCG_AsyncDiv2Clk);
            break;

        case kCLOCK_ScgLpFllAsyncDiv2Clk:
            freq = CLOCK_GetLpFllAsyncFreq(kSCG_AsyncDiv2Clk);
            break;

        case kCLOCK_LpoClk:
            freq = LPO_CLK_FREQ;
            break;
        case kCLOCK_Osc32kClk:
            freq = CLOCK_GetOsc32kClkFreq();
            break;
        case kCLOCK_ErClk:
            freq = CLOCK_GetErClkFreq();
            break;
        default:
            freq = 0U;
            break;
    }
    return freq;
}

uint32_t CLOCK_GetIpFreq(clock_ip_name_t name)
{
    uint32_t reg = (*(volatile uint32_t *)name);

    scg_async_clk_t asycClk;
    uint32_t freq;

    assert(reg & PCC_CLKCFG_PR_MASK);

    asycClk = kSCG_AsyncDiv2Clk;

    switch (PCC_PCS_VAL(reg))
    {
        case kCLOCK_IpSrcSysOscAsync:
            freq = CLOCK_GetSysOscAsyncFreq(asycClk);
            break;
        case kCLOCK_IpSrcSircAsync:
            freq = CLOCK_GetSircAsyncFreq(asycClk);
            break;
        case kCLOCK_IpSrcFircAsync:
            freq = CLOCK_GetFircAsyncFreq(asycClk);
            break;
        case kCLOCK_IpSrcLpFllAsync:
            freq = CLOCK_GetLpFllAsyncFreq(asycClk);
            break;
        default:
            freq = 0U;
            break;
    }

    return freq;
}

void OSC32_Init(OSC32_Type *base, osc32_mode_t mode)
{
    /* Only support one instance now. */
    assert(OSC32 == base);

    CLOCK_EnableClock(kCLOCK_RtcOsc0);

    /* Set work mode. */
    base->CR = (uint8_t)mode;

    if (mode & OSC32_CR_ROSCEN_MASK)
    {
        /* If use crystal mode, wait for stable. */
        while (!(base->CR & OSC32_CR_ROSCSTB_MASK))
        {
        }
    }
}

void OSC32_Deinit(OSC32_Type *base)
{
    /* Only support one instance now. */
    assert(OSC32 == base);

    base->CR = 0U;
    CLOCK_DisableClock(kCLOCK_RtcOsc0);
}

uint32_t CLOCK_GetSysClkFreq(scg_sys_clk_t type)
{
    uint32_t freq;

    scg_sys_clk_config_t sysClkConfig;

    CLOCK_GetCurSysClkConfig(&sysClkConfig);

    switch (sysClkConfig.src)
    {
        case kSCG_SysClkSrcSysOsc:
            freq = CLOCK_GetSysOscFreq();
            break;
        case kSCG_SysClkSrcSirc:
            freq = CLOCK_GetSircFreq();
            break;
        case kSCG_SysClkSrcFirc:
            freq = CLOCK_GetFircFreq();
            break;
        case kSCG_SysClkSrcLpFll:
            freq = CLOCK_GetLpFllFreq();
            break;
        default:
            freq = 0U;
            break;
    }

    freq /= (sysClkConfig.divCore + 1U);

    if (kSCG_SysClkSlow == type)
    {
        freq /= (sysClkConfig.divSlow + 1U);
    }
    else
    {
    }

    return freq;
}

status_t CLOCK_InitSysOsc(const scg_sosc_config_t *config)
{
    assert(config);
    uint8_t range = 0U; /* SCG_SOSCCFG[RANGE] */
    status_t status;
    uint8_t tmp8;

    /* If crystal oscillator used, need to get RANGE value base on frequency. */
    if (kSCG_SysOscModeExt != config->workMode)
    {
        if ((config->freq >= 32768U) && (config->freq <= 40000U))
        {
            range = 1U;
        }
        else if ((config->freq >= 1000000U) && (config->freq <= 8000000U))
        {
            range = 2U;
        }
        else if ((config->freq >= 8000000U) && (config->freq <= 32000000U))
        {
            range = 3U;
        }
        else
        {
            return kStatus_InvalidArgument;
        }
    }

    /* De-init the SOSC first. */
    status = CLOCK_DeinitSysOsc();

    if (kStatus_Success != status)
    {
        return status;
    }

    /* Now start to set up OSC clock. */
    /* Step 1. Setup dividers. */
    SCG->SOSCDIV = SCG_SOSCDIV_SOSCDIV1(config->div1) | SCG_SOSCDIV_SOSCDIV2(config->div2);

    /* Step 2. Set OSC configuration. */
    SCG->SOSCCFG = config->workMode | SCG_SOSCCFG_RANGE(range);

    /* Step 3. Enable clock. */
    /* SCG->SOSCCSR = SCG_SOSCCSR_SOSCEN_MASK | (config->enableMode); */
    tmp8 = config->enableMode;
    tmp8 |= SCG_SOSCCSR_SOSCEN_MASK;
    SCG->SOSCCSR = tmp8;

    /* Step 4. Wait for OSC clock to be valid. */
    while (!(SCG->SOSCCSR & SCG_SOSCCSR_SOSCVLD_MASK))
    {
    }

    /* Step 5. Enabe monitor. */
    SCG->SOSCCSR |= (uint32_t)config->monitorMode;

    return kStatus_Success;
}

status_t CLOCK_DeinitSysOsc(void)
{
    uint32_t reg = SCG->SOSCCSR;

    /* If clock is used by system, return error. */
    if (reg & SCG_SOSCCSR_SOSCSEL_MASK)
    {
        return kStatus_SCG_Busy;
    }

    /* If configure register is locked, return error. */
    if (reg & SCG_SOSCCSR_LK_MASK)
    {
        return kStatus_ReadOnly;
    }

    SCG->SOSCCSR = SCG_SOSCCSR_SOSCERR_MASK;

    return kStatus_Success;
}

uint32_t CLOCK_GetSysOscFreq(void)
{
    if (SCG->SOSCCSR & SCG_SOSCCSR_SOSCVLD_MASK) /* System OSC clock is valid. */
    {
        /* Please call CLOCK_SetXtal0Freq base on board setting before using OSC0 clock. */
        assert(g_xtal0Freq);
        return g_xtal0Freq;
    }
    else
    {
        return 0U;
    }
}

uint32_t CLOCK_GetSysOscAsyncFreq(scg_async_clk_t type)
{
    uint32_t oscFreq = CLOCK_GetSysOscFreq();
    uint32_t divider = 0U;

    /* Get divider. */
    if (oscFreq)
    {
        switch (type)
        {
            case kSCG_AsyncDiv2Clk: /* SOSCDIV2_CLK. */
                divider = SCG_SOSCDIV_SOSCDIV2_VAL;
                break;
            case kSCG_AsyncDiv1Clk: /* SOSCDIV1_CLK. */
                divider = SCG_SOSCDIV_SOSCDIV1_VAL;
                break;
            default:
                break;
        }
    }
    if (divider)
    {
        return oscFreq >> (divider - 1U);
    }
    else /* Output disabled. */
    {
        return 0U;
    }
}

status_t CLOCK_InitSirc(const scg_sirc_config_t *config)
{
    assert(config);

    status_t status;

    /* De-init the SIRC first. */
    status = CLOCK_DeinitSirc();

    if (kStatus_Success != status)
    {
        return status;
    }

    /* Now start to set up SIRC clock. */
    /* Step 1. Setup dividers. */
    SCG->SIRCDIV = SCG_SIRCDIV_SIRCDIV1(config->div1) | SCG_SIRCDIV_SIRCDIV2(config->div2);

    /* Step 2. Set SIRC configuration. */
    SCG->SIRCCFG = SCG_SIRCCFG_RANGE(config->range);

    /* Step 3. Enable clock. */
    SCG->SIRCCSR = SCG_SIRCCSR_SIRCEN_MASK | config->enableMode;

    /* Step 4. Wait for SIRC clock to be valid. */
    while (!(SCG->SIRCCSR & SCG_SIRCCSR_SIRCVLD_MASK))
    {
    }

    return kStatus_Success;
}

status_t CLOCK_DeinitSirc(void)
{
    uint32_t reg = SCG->SIRCCSR;

    /* If clock is used by system, return error. */
    if (reg & SCG_SIRCCSR_SIRCSEL_MASK)
    {
        return kStatus_SCG_Busy;
    }

    /* If configure register is locked, return error. */
    if (reg & SCG_SIRCCSR_LK_MASK)
    {
        return kStatus_ReadOnly;
    }

    SCG->SIRCCSR = 0U;

    return kStatus_Success;
}

uint32_t CLOCK_GetSircFreq(void)
{
    static const uint32_t sircFreq[] = {SCG_SIRC_LOW_RANGE_FREQ, SCG_SIRC_HIGH_RANGE_FREQ};

    if (SCG->SIRCCSR & SCG_SIRCCSR_SIRCVLD_MASK) /* SIRC is valid. */
    {
        return sircFreq[SCG_SIRCCFG_RANGE_VAL];
    }
    else
    {
        return 0U;
    }
}

uint32_t CLOCK_GetSircAsyncFreq(scg_async_clk_t type)
{
    uint32_t sircFreq = CLOCK_GetSircFreq();
    uint32_t divider = 0U;

    /* Get divider. */
    if (sircFreq)
    {
        switch (type)
        {
            case kSCG_AsyncDiv2Clk: /* SIRCDIV2_CLK. */
                divider = SCG_SIRCDIV_SIRCDIV2_VAL;
                break;
            case kSCG_AsyncDiv1Clk: /* SIRCDIV2_CLK. */
                divider = SCG_SIRCDIV_SIRCDIV1_VAL;
                break;
            default:
                break;
        }
    }
    if (divider)
    {
        return sircFreq >> (divider - 1U);
    }
    else /* Output disabled. */
    {
        return 0U;
    }
}

status_t CLOCK_InitFirc(const scg_firc_config_t *config)
{
    assert(config);

    status_t status;

    /* De-init the FIRC first. */
    status = CLOCK_DeinitFirc();

    if (kStatus_Success != status)
    {
        return status;
    }

    /* Now start to set up FIRC clock. */
    /* Step 1. Setup dividers. */
    SCG->FIRCDIV = SCG_FIRCDIV_FIRCDIV1(config->div1) | SCG_FIRCDIV_FIRCDIV2(config->div2);

    /* Step 2. Set FIRC configuration. */
    SCG->FIRCCFG = SCG_FIRCCFG_RANGE(config->range);

    /* Step 3. Set trimming configuration. */
    if (config->trimConfig)
    {
        SCG->FIRCTCFG =
            SCG_FIRCTCFG_TRIMDIV(config->trimConfig->trimDiv) | SCG_FIRCTCFG_TRIMSRC(config->trimConfig->trimSrc);

        /* TODO: Write FIRCSTAT cause bus error: TKT266932. */
        if (kSCG_FircTrimNonUpdate == config->trimConfig->trimMode)
        {
            SCG->FIRCSTAT = SCG_FIRCSTAT_TRIMCOAR(config->trimConfig->trimCoar) |
                            SCG_FIRCSTAT_TRIMFINE(config->trimConfig->trimFine);
        }

        /* trim mode. */
        SCG->FIRCCSR = config->trimConfig->trimMode;

        if (SCG->FIRCCSR & SCG_FIRCCSR_FIRCERR_MASK)
        {
            return kStatus_Fail;
        }
    }

    /* Step 4. Enable clock. */
    SCG->FIRCCSR |= (SCG_FIRCCSR_FIRCEN_MASK | config->enableMode);

    /* Step 5. Wait for FIRC clock to be valid. */
    while (!(SCG->FIRCCSR & SCG_FIRCCSR_FIRCVLD_MASK))
    {
    }

    return kStatus_Success;
}

status_t CLOCK_DeinitFirc(void)
{
    uint32_t reg = SCG->FIRCCSR;

    /* If clock is used by system, return error. */
    if (reg & SCG_FIRCCSR_FIRCSEL_MASK)
    {
        return kStatus_SCG_Busy;
    }

    /* If configure register is locked, return error. */
    if (reg & SCG_FIRCCSR_LK_MASK)
    {
        return kStatus_ReadOnly;
    }

    SCG->FIRCCSR = SCG_FIRCCSR_FIRCERR_MASK;

    return kStatus_Success;
}

uint32_t CLOCK_GetFircFreq(void)
{
    static const uint32_t fircFreq[] = {
        SCG_FIRC_FREQ0, SCG_FIRC_FREQ1, SCG_FIRC_FREQ2, SCG_FIRC_FREQ3,
    };

    if (SCG->FIRCCSR & SCG_FIRCCSR_FIRCVLD_MASK) /* FIRC is valid. */
    {
        return fircFreq[SCG_FIRCCFG_RANGE_VAL];
    }
    else
    {
        return 0U;
    }
}

uint32_t CLOCK_GetFircAsyncFreq(scg_async_clk_t type)
{
    uint32_t fircFreq = CLOCK_GetFircFreq();
    uint32_t divider = 0U;

    /* Get divider. */
    if (fircFreq)
    {
        switch (type)
        {
            case kSCG_AsyncDiv2Clk: /* FIRCDIV2_CLK. */
                divider = SCG_FIRCDIV_FIRCDIV2_VAL;
                break;
            case kSCG_AsyncDiv1Clk: /* FIRCDIV1_CLK. */
                divider = SCG_FIRCDIV_FIRCDIV1_VAL;
                break;
            default:
                break;
        }
    }
    if (divider)
    {
        return fircFreq >> (divider - 1U);
    }
    else /* Output disabled. */
    {
        return 0U;
    }
}

status_t CLOCK_InitLpFll(const scg_lpfll_config_t *config)
{
    assert(config);

    status_t status;

    /* De-init the LPFLL first. */
    status = CLOCK_DeinitLpFll();

    if (kStatus_Success != status)
    {
        return status;
    }

    /* Now start to set up LPFLL clock. */
    /* Step 1. Setup dividers. */
    SCG->LPFLLDIV = SCG_LPFLLDIV_LPFLLDIV1(config->div1) | SCG_LPFLLDIV_LPFLLDIV2(config->div2);

    /* Step 2. Set LPFLL configuration. */
    SCG->LPFLLCFG = SCG_LPFLLCFG_FSEL(config->range);

    /* Step 3. Set trimming configuration. */
    if (config->trimConfig)
    {
        SCG->LPFLLTCFG = SCG_LPFLLTCFG_TRIMDIV(config->trimConfig->trimDiv) |
                         SCG_LPFLLTCFG_TRIMSRC(config->trimConfig->trimSrc) |
                         SCG_LPFLLTCFG_LOCKW2LSB(config->trimConfig->lockMode);

        if (kSCG_LpFllTrimNonUpdate == config->trimConfig->trimMode)
        {
            SCG->LPFLLSTAT = config->trimConfig->trimValue;
        }

        /* Trim mode. */
        SCG->LPFLLCSR = config->trimConfig->trimMode;

        if (SCG->LPFLLCSR & SCG_LPFLLCSR_LPFLLERR_MASK)
        {
            return kStatus_Fail;
        }
    }

    /* Step 4. Enable clock. */
    SCG->LPFLLCSR |= (SCG_LPFLLCSR_LPFLLEN_MASK | config->enableMode);

    /* Step 5. Wait for LPFLL clock to be valid. */
    while (!(SCG->LPFLLCSR & SCG_LPFLLCSR_LPFLLVLD_MASK))
    {
    }

    /* Step 6. Wait for LPFLL trim lock. */
    if ((config->trimConfig) && (kSCG_LpFllTrimUpdate == config->trimConfig->trimMode))
    {
        while (!(SCG->LPFLLCSR & SCG_LPFLLCSR_LPFLLTRMLOCK_MASK))
        {
        }
    }

    return kStatus_Success;
}

status_t CLOCK_DeinitLpFll(void)
{
    uint32_t reg = SCG->LPFLLCSR;

    /* If clock is used by system, return error. */
    if (reg & SCG_LPFLLCSR_LPFLLSEL_MASK)
    {
        return kStatus_SCG_Busy;
    }

    /* If configure register is locked, return error. */
    if (reg & SCG_LPFLLCSR_LK_MASK)
    {
        return kStatus_ReadOnly;
    }

    SCG->LPFLLCSR = SCG_LPFLLCSR_LPFLLERR_MASK;

    return kStatus_Success;
}

uint32_t CLOCK_GetLpFllFreq(void)
{
    static const uint32_t lpfllFreq[] = {
        SCG_LPFLL_FREQ0, SCG_LPFLL_FREQ1, SCG_LPFLL_FREQ2, SCG_LPFLL_FREQ3,
    };

    if (SCG->LPFLLCSR & SCG_LPFLLCSR_LPFLLVLD_MASK) /* LPFLL is valid. */
    {
        return lpfllFreq[SCG_LPFLLCFG_FSEL_VAL];
    }
    else
    {
        return 0U;
    }
}

uint32_t CLOCK_GetLpFllAsyncFreq(scg_async_clk_t type)
{
    uint32_t lpfllFreq = CLOCK_GetLpFllFreq();
    uint32_t divider = 0U;

    /* Get divider. */
    if (lpfllFreq)
    {
        switch (type)
        {
            case kSCG_AsyncDiv2Clk: /* LPFLLDIV2_CLK. */
                divider = SCG_LPFLLDIV_LPFLLDIV2_VAL;
                break;
            case kSCG_AsyncDiv1Clk: /* LPFLLDIV1_CLK. */
                divider = SCG_LPFLLDIV_LPFLLDIV1_VAL;
                break;
            default:
                break;
        }
    }
    if (divider)
    {
        return lpfllFreq >> (divider - 1U);
    }
    else /* Output disabled. */
    {
        return 0U;
    }
}
