#ifndef __BSP_SPI_H
#define __BSP_SPI_H

#include "stdint.h"

void SPI1_DMA_init(uint32_t tx_buf, uint32_t rx_buf, uint16_t num);
void SPI1_DMA_enable(uint32_t tx_buf, uint32_t rx_buf, uint16_t ndtr);

#endif
