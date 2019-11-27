#include "adc_vbat.h"

static void adc_calib(){
  if ((ADC1->CR & ADC_CR_ADEN) != 0)
    ADC1->CR |= ADC_CR_ADDIS;
  while ((ADC1->CR & ADC_CR_ADEN) != 0){};
  ADC1->CFGR1 &= ~ADC_CFGR1_DMAEN;
  ADC1->CR |= ADC_CR_ADCAL;
  while ((ADC1->CR & ADC_CR_ADCAL) !=0){};
}

void adc_config(){
  RCC->AHBENR |= RCC_AHBENR_GPIOAEN;                              
  GPIO_SET_ANALOG(GPIOA,0);

  RCC->APB2ENR |= RCC_APB2ENR_ADCEN;
  adc_calib();
  
  ADC1->CFGR1 |= ADC_CFGR1_CONT;
  ADC1->CFGR2 |= ADC_CFGR2_CKMODE_1;//frq = clk/4
  ADC1->SMPR   |= ADC_SMPR_SMP_0 | ADC_SMPR_SMP_1 | ADC_SMPR_SMP_2;//max smplng time
  ADC1->CHSELR  = ADC_CHSELR_CHSEL0;
  ADC1->CFGR2  |= ADC_CFGR2_CKMODE_0;
  ADC1->CR |= ADC_CR_ADEN;
  ADC1->CR |= ADC_CR_ADSTART;
}

uint16_t adc_vbat_value(){
  return ADC1->DR;
}



