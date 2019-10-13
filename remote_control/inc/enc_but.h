#ifndef __ENCBUT__
#define __ENCBUT__

#include "gpio_macros.h"
#include "stm32f0xx.h"


extern uint16_t enc_buff[];
extern uint16_t shift_reg[];
extern uint16_t leds[];

uint16_t read_but();
void EXTI_config();
void EXTI4_15_IRQHandler();
void led_fill(uint16_t *regs);
#endif