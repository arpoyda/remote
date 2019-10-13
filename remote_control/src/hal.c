#include "hal.h"


void rcc_config(){
  RCC_HSEConfig(RCC_HSE_ON);
  if (RCC_WaitForHSEStartUp() == SUCCESS){
    RCC_SYSCLKConfig( RCC_SYSCLKSource_HSE);
    while(RCC_GetSYSCLKSource() != 0x04); 
    RCC_HSICmd(DISABLE);
  } else { 
    RCC_HSICmd(ENABLE);
    RCC_SYSCLKConfig(RCC_SYSCLKSource_HSI);
    while(RCC_GetSYSCLKSource() != 0x00){};
    RCC_HSEConfig(RCC_HSE_OFF);
  }
#ifdef _PLLON16  
  RCC_PLLConfig(RCC_PLLSource_HSI_Div2, RCC_PLLMul_4); //16Mhz
  RCC_PLLCmd(ENABLE);
  while((RCC->CR & (1L<<25)) == 0);
  RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
  while(RCC_GetSYSCLKSource() != 0x08);  
#endif  
  RCC_HCLKConfig(RCC_SYSCLK_Div1);
  RCC_PCLKConfig(RCC_HCLK_Div1);  
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA | RCC_AHBPeriph_GPIOB |
                        RCC_AHBPeriph_GPIOF | RCC_AHBPeriph_GPIOC, ENABLE);
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
  RCC_APB2PeriphClockCmd(/*RCC_APB2Periph_ADC1| */RCC_APB2Periph_SYSCFG  ,ENABLE);
  
  GPIO_SET_PUPUP(GPIOB,0);
  GPIO_SET_PUPUP(GPIOB,1);
  GPIO_SET_PUPUP(GPIOB,3);
  GPIO_SET_PUPUP(GPIOB,4);
  GPIO_SET_PUPUP(GPIOB,7);
  GPIO_SET_PUPUP(GPIOB,10);
  GPIO_SET_PUPUP(GPIOA,13);
  GPIO_SET_PUPUP(GPIOA,14);
  GPIO_SET_PUPUP(GPIOA,15);
  
  GPIO_SET_OUTPUT(GPIOF,7);
  GPIO_SET_BIT(GPIOF, 7);
}

void start_delay(){
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM16,ENABLE);
  TIM16->PSC = 16000 - 1;
  TIM16->ARR = 50; // 0.05sec
  //TIM16->DIER |= TIM_DIER_UIE;
  TIM16->CR1 |= TIM_CR1_CEN;
  while(TIM16->CNT < (TIM16->ARR)-1);
  TIM16->CNT = 0;
  TIM16->SR &= ~TIM_SR_UIF; //???
  TIM16->CR1 &= ~TIM_CR1_CEN;
}

void turn_off(){
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM15,ENABLE);
  TIM15->PSC = 1600000 - 1;//10Hz
  TIM15->ARR = 10; // 0.05sec
  TIM15->DIER |= TIM_DIER_UIE;
  TIM15->CR1 |= TIM_CR1_CEN;
}
void TIM15_IRQHandler(void){
  TIM15->SR &= ~TIM_SR_UIF;
  static uint16_t counter = 0;
  if (GPIO_READ_BIT(GPIOF, 6))
    counter++;
  if (counter >= 5)
    GPIO_RESET_BIT(GPIOF, 7);
}

int16_t read_register(int16_t addr){
  if (addr<ENC_NUM) {
    return enc_buff[addr];
  } else
  if (addr < ENC_NUM+1){
    return read_but();
  } else
  if (addr < REG_NUM+ENC_NUM+1){
    return leds[addr-ENC_NUM-1];
  }
  return -1;
}


int16_t write_register(int16_t addr, int16_t data){
  if (addr<ENC_NUM+1) {
    enc_buff[addr] = data;
    return 1;
  } else
  if (addr < ENC_NUM+2){
    //buttoms are only for reading
    return 0;
  } else
  if (addr < REG_NUM+ENC_NUM+2){
    leds[addr-ENC_NUM-2] = data;
    led_fill(leds);
    return 1;
  }
  return 0;
}

void spi_tx_proc(){
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM16,ENABLE);
  TIM16->PSC = 8000 - 1;//1kHz
  TIM16->ARR = 25;
  TIM16->DIER |= TIM_DIER_UIE;
  NVIC_SetPriority(TIM16_IRQn,3); 
  NVIC_EnableIRQ(TIM16_IRQn);
  TIM16->CR1 |= TIM_CR1_CEN;
}

void TIM16_IRQHandler(void){
  TIM16->SR &= ~TIM_SR_UIF;
  //spi1_tx_ie(leds, 3);
  spi1_tx_ie(shift_reg, 9);
}
