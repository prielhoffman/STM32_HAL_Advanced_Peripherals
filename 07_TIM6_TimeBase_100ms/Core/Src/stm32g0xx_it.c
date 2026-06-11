#include "main.h"

extern TIM_HandleTypeDef htim6;

void SysTick_Handler(void){
  HAL_IncTick();
}

void TIM6_DAC_LPTIM1_IRQHandler(void){
  HAL_TIM_IRQHandler(&htim6);
}
