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

void HAL_TIM_PWM_MspInit(TIM_HandleTypeDef* htim){
	if (htim->Instance == TIM2){
		/* Enable peripheral clocks for Timer 2 and GPIO Ports */
		__HAL_RCC_TIM2_CLK_ENABLE();
		__HAL_RCC_GPIOA_CLK_ENABLE();
		__HAL_RCC_GPIOB_CLK_ENABLE();

		GPIO_InitTypeDef GPIO_InitStruct = {0};

		/* Configure PA0 (CH1) and PA1 (CH2) as TIM2 Alternate Function */
		GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
		GPIO_InitStruct.Alternate = GPIO_AF2_TIM2;
		GPIO_InitStruct.Pin = GPIO_PIN_0;
		HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
	}
}
