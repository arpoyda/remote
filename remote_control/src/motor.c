#include "motor.h"

void TIM1_config(){
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);  
  //CH1,2,3
  GPIO_SET_AF(GPIOA,8,1);
  GPIO_SET_AF(GPIOA,9,1);
  GPIO_SET_AF(GPIOA,10,1);
  //CH1N,2N,3N
  GPIO_SET_AF(GPIOB,13,1);
  GPIO_SET_AF(GPIOB,14,1);
  GPIO_SET_AF(GPIOB,15,1);

  RCC->APB2ENR |= RCC_APB2ENR_TIM1EN;
  TIM1->PSC = PSC_VALUE;
  TIM1->CCMR1 = TIM_CCMR1_OC1M_2 | TIM_CCMR1_OC1M_1 | TIM_CCMR1_OC1PE |
                TIM_CCMR1_OC2M_2 | TIM_CCMR1_OC2M_1 | TIM_CCMR1_OC2PE;
  TIM1->CCMR2 = TIM_CCMR2_OC3M_2 | TIM_CCMR2_OC3M_1 | TIM_CCMR2_OC3PE;
  TIM1->CCER = TIM_CCER_CC1E | TIM_CCER_CC1NE |TIM_CCER_CC2E | TIM_CCER_CC2NE|
               TIM_CCER_CC3E | TIM_CCER_CC3NE;
  TIM1->BDTR = TIM_BDTR_MOE | DEADTIME;
  TIM1->CCR1 = 0; // changeable
  TIM1->CCR2 = 0;
  TIM1->CCR3 = 0;
  TIM1->ARR = T_VALUE;
  TIM1->CR1 = TIM_CR1_ARPE;
  //TIM1->CR1| = TIM_CR1_CEN;
  TIM1->EGR = TIM_EGR_UG; //?
};

void motor_on(){
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM16,ENABLE);
  TIM16->PSC = 8000 - 1;
  TIM16->ARR = 100;
  TIM16->DIER |= TIM_DIER_UIE;
  //TIM16->CR1 |= TIM_CR1_CEN;
  NVIC_SetPriority(TIM16_IRQn,3); 
  NVIC_EnableIRQ(TIM16_IRQn);
  
  for (int i=0; i<3; i++){
    tim1_ctrl.ch[i] = 0;
    tim1_ctrl.cnt[i] = 0;
    tim1_ctrl.isinc[i] = 0;
  GPIO_SET_OUTPUT(GPIOA, 6);
}

void TIM16_IRQHandler(void){
  TIM16->SR &= ~TIM_SR_UIF;
  if (tim1_ctrl.ch[0] && tim1_ctrl.isinc[0]){
    TIM->CCR1 += 5;
    if (TIM->CCR1 == T_VALUE/2)
      tim1_ctrl.isinc[0] = 0;
  }
  if (!tim1_ctrl.ch[0] && !tim1_ctrl.isinc[0]){
    TIM->CCR1 += 5;
    if (TIM->CCR1 == T_VALUE/2)
      tim1_ctrl.isinc[0] = 1;
  }
  if (!tim1_ctrl.ch[0] && tim1_ctrl.isinc[0]){
    TIM->CCR1 += 5;
    if (TIM->CCR1 != 0)
      tim1_ctrl.isinc[0] = 0;
  }
}
