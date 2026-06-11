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

void HAL_TIM_Base_MspInit(TIM_HandleTypeDef* htim_base){
	if (htim_base->Instance == TIM6){
	    /* Enable the peripheral clock for TIM6 */
		__HAL_RCC_TIM6_CLK_ENABLE();
		/* Enable TIM6 interrupt in the NVIC */
		HAL_NVIC_SetPriority(TIM6_DAC_LPTIM1_IRQn, 0, 0);
		HAL_NVIC_EnableIRQ(TIM6_DAC_LPTIM1_IRQn);
	}
}
