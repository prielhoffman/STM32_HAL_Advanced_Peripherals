#include "main.h"

void HAL_MspInit(void)
{
  /*
   * Enable system configuration clock.
   * Bare-metal idea: enable SYSCFG clock in RCC.
   */
  __HAL_RCC_SYSCFG_CLK_ENABLE();

  /*
   * Enable power interface clock.
   * Bare-metal idea: enable PWR clock in RCC.
   */
  __HAL_RCC_PWR_CLK_ENABLE();
}

void HAL_TIM_IC_MspInit(TIM_HandleTypeDef* htim_ic){
	if (htim_ic->Instance == TIM2){

		__HAL_RCC_TIM2_CLK_ENABLE();

		HAL_NVIC_SetPriority(TIM2_IRQn, 0, 0);
		HAL_NVIC_EnableIRQ(TIM2_IRQn);
	}
}
