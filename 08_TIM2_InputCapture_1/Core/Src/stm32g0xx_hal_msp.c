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

		/* Enable peripheral clocks */
		__HAL_RCC_TIM2_CLK_ENABLE();
		__HAL_RCC_GPIOA_CLK_ENABLE();

		/* Configure PA0 as TIM2_CH1 Alternate Function */
		GPIO_InitTypeDef GPIO_InitStruct = {0};
		GPIO_InitStruct.Pin = GPIO_PIN_0;
		GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;       /* Alternate Function Push-Pull */
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
		GPIO_InitStruct.Alternate = GPIO_AF2_TIM2;    /* Map PA0 specifically to TIM2 Channels */

		HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

		/* Configure NVIC for Timer 2 Interrupts */
		HAL_NVIC_SetPriority(TIM2_IRQn, 0, 0);
		HAL_NVIC_EnableIRQ(TIM2_IRQn);
	}
}

void HAL_UART_MspInit(UART_HandleTypeDef* huart) {
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    if(huart->Instance == USART2) {
        __HAL_RCC_USART2_CLK_ENABLE();
        __HAL_RCC_GPIOA_CLK_ENABLE();

        GPIO_InitStruct.Pin = GPIO_PIN_2 | GPIO_PIN_3;
        GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
        GPIO_InitStruct.Alternate = GPIO_AF1_USART2;

        HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
    }
}
