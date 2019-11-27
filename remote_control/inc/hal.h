#ifndef __HAL__
#define __HAL__

#include "gpio_macros.h"
#include "stm32f0xx_rcc.h"
#include "enc_but.h"
#include "spi.h"
#include "adc_vbat.h"

#define _PLLON16


#define ENC_NUM 3
#define REG_NUM 3




static uint8_t conversion_turnon;
static uint16_t conver_num;
static uint32_t adc_counter;

void turn_off();
void rcc_config();
void start_delay();
void spi_tx_proc();
int16_t read_register(int16_t addr);
int16_t write_register(int16_t addr, int16_t data);
#endif
