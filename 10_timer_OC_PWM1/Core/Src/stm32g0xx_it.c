#include "main.h"

extern TIM_HandleTypeDef htimer2;

void SysTick_Handler(void){
  HAL_IncTick();
}
