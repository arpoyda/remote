#include "enc_but.h"

uint16_t enc_buff[3] = {0,0,0};
uint16_t leds[3] = {0,0,0};
uint16_t shift_reg[9] = {0,0,0,0,0,0,0,0,0};

void EXTI_config(){
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);

  GPIO_SET_PUPUP(GPIOB,5);//En-A
  GPIO_SET_PUPUP(GPIOB,6);//En-B--

  GPIO_SET_PUPUP(GPIOB,8);//En-A
  GPIO_SET_PUPUP(GPIOB,9);//En-B--

  GPIO_SET_PUPUP(GPIOB,11);//En-A
  GPIO_SET_PUPUP(GPIOB,12);//En-B--
  
  //------ENCODER------//
  SYSCFG->EXTICR[1] |= SYSCFG_EXTICR2_EXTI5_PB | SYSCFG_EXTICR2_EXTI6_PB;
  SYSCFG->EXTICR[2] |= SYSCFG_EXTICR3_EXTI8_PB | SYSCFG_EXTICR3_EXTI9_PB |
                       SYSCFG_EXTICR3_EXTI11_PB;
  SYSCFG->EXTICR[3] |= SYSCFG_EXTICR4_EXTI12_PB;
  EXTI->RTSR |= EXTI_RTSR_TR5 | EXTI_RTSR_TR6 | EXTI_RTSR_TR8 |
                EXTI_RTSR_TR9 | EXTI_RTSR_TR11| EXTI_RTSR_TR12;
  EXTI->FTSR |= EXTI_FTSR_TR5 | EXTI_FTSR_TR6 | EXTI_FTSR_TR8 |
                EXTI_FTSR_TR9 | EXTI_FTSR_TR11| EXTI_FTSR_TR12;
  EXTI->PR |= EXTI_PR_PR5 | EXTI_PR_PR6 | EXTI_PR_PR8 |
              EXTI_PR_PR9 | EXTI_PR_PR11 | EXTI_PR_PR12;
  EXTI->IMR |= EXTI_IMR_MR5 | EXTI_IMR_MR6 | EXTI_IMR_MR8 |
               EXTI_IMR_MR9 | EXTI_IMR_MR11 | EXTI_IMR_MR12;
  //-----INTERRUPT-----//
  NVIC_SetPriority(EXTI4_15_IRQn, 2);
  NVIC_EnableIRQ(EXTI4_15_IRQn);


}

void EXTI4_15_IRQHandler(){
  //-----ENCODER-----//
  static int8_t states[] = {0,1,-1,0,-1,0,0,1,1,0,0,-1,0,-1,1,0};
  static uint8_t enc_trans = 0;
  uint16_t enc_state = 0x00;
  
  if (EXTI->PR & EXTI_PR_PR5 | EXTI->PR & EXTI_PR_PR6){
    EXTI->PR |= EXTI_PR_PR5 | EXTI_PR_PR6;
    enc_state = (GPIO_READ_BIT(GPIOB, 5) |  (GPIO_READ_BIT(GPIOB, 6)))>>5;
    enc_trans = ((0x03 & enc_trans) << 2) | enc_state;
    if (states[enc_trans]==1){
      enc_buff[0]++;
    }
    if (states[enc_trans]==-1){
      enc_buff[0]--;
    }
  } else if (EXTI->PR & EXTI_PR_PR8 | EXTI->PR & EXTI_PR_PR9){
    EXTI->PR |= EXTI_PR_PR8 | EXTI_PR_PR9;
    enc_state = (GPIO_READ_BIT(GPIOB, 8) |  (GPIO_READ_BIT(GPIOB, 9)))>>8;
    enc_trans = ((0x03 & enc_trans) << 2) | enc_state;
    if (states[enc_trans]==1){
      enc_buff[1]++;
    }
    if (states[enc_trans]==-1){
      enc_buff[1]--;
    }
  } else if (EXTI->PR & EXTI_PR_PR11 | EXTI->PR & EXTI_PR_PR12){
    EXTI->PR |= EXTI_PR_PR11 | EXTI_PR_PR12;
    enc_state = (GPIO_READ_BIT(GPIOB, 11) |  (GPIO_READ_BIT(GPIOB, 12)))>>11;
    enc_trans = ((0x03 & enc_trans) << 2) | enc_state;
    if (states[enc_trans]==1){
      enc_buff[2]++;
    }
    if (states[enc_trans]==-1){
      enc_buff[2]--;
    }
  }
}

uint16_t read_but(){
  uint16_t but_status = 0;
  but_status = GPIO_READ_BIT(GPIOB, 0) | GPIO_READ_BIT(GPIOB, 1)  |
               GPIO_READ_BIT(GPIOB, 3) | GPIO_READ_BIT(GPIOA, 13) |
               GPIO_READ_BIT(GPIOA, 14)| GPIO_READ_BIT(GPIOA, 15) |
               GPIO_READ_BIT(GPIOB, 4) | GPIO_READ_BIT(GPIOB, 7)  |
               GPIO_READ_BIT(GPIOB, 10);
  return but_status;
}

void led_fill(uint16_t *regs){
  shift_reg[0] = regs[0];
  shift_reg[1] = regs[1];
  shift_reg[2] = regs[2];
  
  shift_reg[3] = regs[0] | 0x1E0;
  shift_reg[4] = regs[1] | 0x1E0;
  shift_reg[5] = regs[2] | 0x1E0;
  shift_reg[6] = regs[0] | 0x3E0;
  shift_reg[7] = regs[1] | 0x3E0;
  shift_reg[8] = regs[2] | 0x3E0;
}