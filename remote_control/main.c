#include "stm32f0xx.h"
#include "core_cm0.h"
#include "stm32f0xx_rcc.h"
#include "stm32f0xx_iwdg.h"
#include "hal.h"
#include "modbus.h"
#include "usart.h"
#include "enc_but.h"
#include "spi.h"


void SystemInit(void)
{
}

int main()
{
  rcc_config();
  modbus_init(1);
  __enable_irq();
  EXTI_config();
  spi_mstr_config();
  
  start_delay();
  turn_off();
  spi_tx_proc();
  
  while(1){
    IWDG_ReloadCounter();
  }
}
