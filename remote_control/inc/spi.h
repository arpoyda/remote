#ifndef __SPI__
#define __SPI__

#include "stm32f0xx.h"
#include "gpio_macros.h"
#include "enc_but.h"


void spi_mstr_config(void);
void spi1_tx_dma(uint16_t* data, uint32_t data_len);
void spi1_tx_byte(uint16_t data);
void spi1_tx_ie(uint16_t* data, int32_t data_len);
#endif
