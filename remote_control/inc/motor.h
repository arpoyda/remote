#ifndef __MOTOR__
#define __MOTOR__

#include "stm32f0xx.h"
#include "stm32f0xx_rcc.h"
#include "gpio_macros.h"

#define PSC_VALUE 0 // 8MHz
#define T_VALUE 400 // 20kHz
#define DEADTIME 10 //


struct tim1_control{
  uint8_t ch[3];
  uint8_t cnt[3];
  uint8_t isinc[3];
};
struct tim1_control tim1_ctrl;

void TIM1_config();
#endif