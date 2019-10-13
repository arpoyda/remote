#include "spi.h"

int32_t tx_index = 0;
int32_t tx_len = 0; 
uint16_t* tx_data;

void spi_mstr_config(void){
  //=================================GPIOA-AF===================================
  RCC_AHBPeriphClockCmd (RCC_AHBPeriph_GPIOA , ENABLE);
  
  GPIO_SET_OUTPUT(GPIOA,4);
  GPIO_SET_AF(GPIOA, 5, 0);
  GPIO_SET_AF(GPIOA, 7, 0);
  GPIO_SET_OUTPUT(GPIOA,6);
  
  GPIO_SET_BIT(GPIOA, 6);
  //=================================SPI-CFG====================================
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);
  
  SPI1->CR1 =  SPI_CR1_SSM | SPI_CR1_SSI | SPI_CR1_BR | SPI_CR1_MSTR;
  SPI1->CR1 &= ~(SPI_CR1_CPHA | SPI_CR1_CPOL);
  SPI1->CR2 |= SPI_CR2_DS;
  //===============================SPI-NVIC=====================================
  NVIC_EnableIRQ(SPI1_IRQn);
  NVIC_SetPriority(SPI1_IRQn, 5);
  //===============================SPI-DMA======================================
  //RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
  //SPI1->CR2 |= SPI_CR2_TXDMAEN;
  
  //=============================SPI-ENABLE=====================================
  SPI1->CR1 |= SPI_CR1_SPE;
  
  GPIO_RESET_BIT(GPIOA, 4);
  leds[0] = 0x0000;
  led_fill(leds);
}

void spi1_tx_dma(uint16_t* data, uint32_t data_len){
  DMA1_Channel3->CCR &= ~DMA_CCR_EN;

  DMA1_Channel3->CPAR = (uint32_t)(&(SPI1->DR));
  DMA1_Channel3->CMAR = (uint32_t)data;
  DMA1_Channel3->CNDTR = data_len;

  DMA1_Channel3->CCR |= DMA_CCR_MINC | DMA_CCR_DIR;

  DMA1_Channel3->CCR |= DMA_CCR_EN;
}

void spi1_tx_byte(uint16_t data){
  while(!(SPI1->SR & SPI_SR_TXE)){};
  SPI1->DR = data;
}

void spi1_tx_ie(uint16_t* data, int32_t data_len){
  if (data_len <= 0)
    return;
  
  while (SPI1->SR & SPI_SR_BSY){};
  
  tx_index = 0;
  tx_len = data_len;
  tx_data = data;
  
  SPI1->CR2 |= SPI_CR2_TXEIE;
}

void SPI1_IRQHandler(void){
  static int i;
  GPIO_RESET_BIT(GPIOA, 6);
  SPI1->DR = tx_data[tx_index];
  if((tx_index+1) % 3 == 0){
    for (i=0; i<1500; i++){};
    GPIO_SET_BIT(GPIOA, 6);
  }
  tx_index++;
  
  if (tx_index >= tx_len)
    SPI1->CR2 &= ~(SPI_CR2_TXEIE); 
}