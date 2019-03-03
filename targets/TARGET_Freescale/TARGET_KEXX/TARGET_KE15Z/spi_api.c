/* mbed Microcontroller Library
 * Copyright (c) 2013 ARM Limited
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
#include <math.h>
#include "mbed_assert.h"

#include "spi_api.h"

#if DEVICE_SPI

#include "cmsis.h"
#include "pinmap.h"
#include "mbed_error.h"
#include "fsl_lpspi.h"
#include "peripheral_clock_defines.h"
#include "PeripheralPins.h"

/* Array of SPI peripheral base address. */
static LPSPI_Type *const spi_address[] = LPSPI_BASE_PTRS;
/* Array of SPI bus clock frequencies */
static clock_ip_name_t const spi_clocks[] = LPSPI_CLOCKS;

void spi_init(spi_t *obj, PinName mosi, PinName miso, PinName sclk, PinName ssel)
{
    // determine the SPI to use
    uint32_t spi_mosi = pinmap_peripheral(mosi, PinMap_SPI_MOSI);
    uint32_t spi_miso = pinmap_peripheral(miso, PinMap_SPI_MISO);
    uint32_t spi_sclk = pinmap_peripheral(sclk, PinMap_SPI_SCLK);
    uint32_t spi_ssel = pinmap_peripheral(ssel, PinMap_SPI_SSEL);
    uint32_t spi_data = pinmap_merge(spi_mosi, spi_miso);
    uint32_t spi_cntl = pinmap_merge(spi_sclk, spi_ssel & ~(0xf0));

    obj->instance = pinmap_merge(spi_data, spi_cntl);
    MBED_ASSERT((int)obj->instance != NC);

    //select the correct instance for PCS
    obj->instance |= (spi_ssel & 0xf0);

    // pin out the spi pins
    pinmap_pinout(mosi, PinMap_SPI_MOSI);
    pinmap_pinout(miso, PinMap_SPI_MISO);
    pinmap_pinout(sclk, PinMap_SPI_SCLK);
    if (ssel != NC) {
        pinmap_pinout(ssel, PinMap_SPI_SSEL);
    }

    //set the clock source of lpspi
    CLOCK_SetIpSrc(spi_clocks[obj->instance >> LPSPI_INSTANCE_SHIFT], kCLOCK_IpSrcFircAsync);
}

void spi_free(spi_t *obj)
{
    LPSPI_Deinit(spi_address[obj->instance >> LPSPI_INSTANCE_SHIFT]);
}

void spi_format(spi_t *obj, int bits, int mode, int slave)
{
    lpspi_master_config_t master_config;
    lpspi_slave_config_t slave_config;

    if (slave) {
        /* Slave config */
        LPSPI_SlaveGetDefaultConfig(&slave_config);
        slave_config.bitsPerFrame = (uint32_t)bits;
        slave_config.cpol = (mode & 0x2) ? kLPSPI_ClockPolarityActiveLow : kLPSPI_ClockPolarityActiveHigh;
        slave_config.cpha = (mode & 0x1) ? kLPSPI_ClockPhaseSecondEdge : kLPSPI_ClockPhaseFirstEdge;

        LPSPI_SlaveInit(spi_address[obj->instance >> LPSPI_INSTANCE_SHIFT], &slave_config);
    } else {
        /* Master config */
        LPSPI_MasterGetDefaultConfig(&master_config);
        master_config.bitsPerFrame = (uint32_t)bits;
        master_config.cpol = (mode & 0x2) ? kLPSPI_ClockPolarityActiveLow : kLPSPI_ClockPolarityActiveHigh;
        master_config.cpha = (mode & 0x1) ? kLPSPI_ClockPhaseSecondEdge : kLPSPI_ClockPhaseFirstEdge;
        master_config.direction = kLPSPI_MsbFirst;

        master_config.whichPcs = (obj->instance & 0xf0) >> LPSPI_PCS_SHIFT;

        LPSPI_MasterInit(spi_address[obj->instance >> LPSPI_INSTANCE_SHIFT], &master_config, CLOCK_GetIpFreq(spi_clocks[obj->instance >> LPSPI_INSTANCE_SHIFT]));
        spi_address[obj->instance >> LPSPI_INSTANCE_SHIFT]->CR |= LPSPI_CR_DBGEN_MASK;
    }
}

void spi_frequency(spi_t *obj, int hz)
{
    uint32_t lpspiClock = CLOCK_GetIpFreq(spi_clocks[obj->instance >> LPSPI_INSTANCE_SHIFT]);
    uint32_t tcrPrescaleValue = 0;

    /* Disable the LPSPI module before setting the baudrate */
    LPSPI_Enable(spi_address[obj->instance >> LPSPI_INSTANCE_SHIFT], false);
    LPSPI_MasterSetBaudRate(spi_address[obj->instance >> LPSPI_INSTANCE_SHIFT], (uint32_t)hz, lpspiClock, &tcrPrescaleValue);
    //Half clock period delay after SPI transfer
    //*** is this line needed?
    //LPSPI_MasterSetDelayTimes(spi_address[obj->instance >> LPSPI_INSTANCE_SHIFT], kLPSPI_LastSckToPcs, lpspiClock, 500000000 / hz);

    spi_address[obj->instance >> LPSPI_INSTANCE_SHIFT]->TCR = (spi_address[obj->instance >> LPSPI_INSTANCE_SHIFT]->TCR & ~LPSPI_TCR_PRESCALE_MASK) | LPSPI_TCR_PRESCALE(tcrPrescaleValue);

    /* Enable the LPSPI module */
    LPSPI_Enable(spi_address[obj->instance >> LPSPI_INSTANCE_SHIFT], true);
}

static inline int spi_readable(spi_t * obj)
{
    return (LPSPI_GetStatusFlags(spi_address[obj->instance >> LPSPI_INSTANCE_SHIFT]) & kLPSPI_RxDataReadyFlag);
}

int spi_master_write(spi_t *obj, int value)
{
    lpspi_transfer_t masterXfer;
    uint32_t rx_data;

    masterXfer.txData = (uint8_t *)&value;
    masterXfer.rxData = (uint8_t *)&rx_data;
    masterXfer.dataSize = 1;
    //pcs information stored in the instance will be used to select the correct pcs flag bits
    masterXfer.configFlags = (obj->instance & 0xf0) | kLPSPI_MasterPcsContinuous | kLPSPI_SlaveByteSwap;

    LPSPI_MasterTransferBlocking(spi_address[obj->instance >> LPSPI_INSTANCE_SHIFT], &masterXfer);

    // wait rx buffer full
    //while (!spi_readable(obj));
    //rx_data = LPSPI_ReadData(spi_address[obj->instance >> LPSPI_INSTANCE_SHIFT]);

    LPSPI_ClearStatusFlags(spi_address[obj->instance >> LPSPI_INSTANCE_SHIFT], kLPSPI_TransferCompleteFlag);

    return rx_data & 0xffff;
}

int spi_master_block_write(spi_t *obj, const char *tx_buffer, int tx_length,
                           char *rx_buffer, int rx_length, char write_fill) {
    int total = (tx_length > rx_length) ? tx_length : rx_length;

    // Default write is done in each and every call, in future can create HAL API instead
    LPSPI_SetDummyData(spi_address[obj->instance >> LPSPI_INSTANCE_SHIFT], write_fill);

    LPSPI_MasterTransferBlocking(spi_address[obj->instance >> LPSPI_INSTANCE_SHIFT], &(lpspi_transfer_t){
          .txData = (uint8_t *)tx_buffer,
          .rxData = (uint8_t *)rx_buffer,
          .dataSize = total,
		  //pcs information stored in the instance will be used to select the correct pcs flag bits
          .configFlags = (obj->instance & 0xf0) | kLPSPI_MasterPcsContinuous | kLPSPI_SlaveByteSwap,
    });

    return total;
}

int spi_slave_receive(spi_t *obj)
{
    return spi_readable(obj);
}

int spi_slave_read(spi_t *obj)
{
    uint32_t rx_data;

    while (!spi_readable(obj));
    rx_data = LPSPI_ReadData(spi_address[obj->instance >> LPSPI_INSTANCE_SHIFT]);

    return rx_data & 0xffff;
}

void spi_slave_write(spi_t *obj, int value)
{
    /*Write the word to TX register*/
    LPSPI_WriteData(spi_address[obj->instance >> LPSPI_INSTANCE_SHIFT], (uint32_t)value);

    /* Transfer is not complete until transfer complete flag sets */
    while (!(LPSPI_GetStatusFlags(spi_address[obj->instance >> LPSPI_INSTANCE_SHIFT]) & kLPSPI_FrameCompleteFlag))
    {
    }

    LPSPI_ClearStatusFlags(spi_address[obj->instance >> LPSPI_INSTANCE_SHIFT], kLPSPI_FrameCompleteFlag);
}

#endif
