#include "main.h"
#include <stdio.h>

/* Function prototype */
static void SystemClock_PLL_64MHz_Config(void);
static void TIMER2_Init(void);


/* Global peripheral handle for Timer 2 */
TIM_HandleTypeDef htimer2;

int main(void){
	HAL_Init();
	SystemClock_PLL_64MHz_Config();

	TIMER2_Init();

	if (HAL_TIM_PWM_Start(&htimer2,TIM_CHANNEL_1) != HAL_OK){
		Error_Handler();
	}
	if (HAL_TIM_PWM_Start(&htimer2,TIM_CHANNEL_2) != HAL_OK){
		Error_Handler();
	}
	if (HAL_TIM_PWM_Start(&htimer2,TIM_CHANNEL_3) != HAL_OK){
		Error_Handler();
	}
	if (HAL_TIM_PWM_Start(&htimer2,TIM_CHANNEL_4) != HAL_OK){
		Error_Handler();
	}

	while (1){

	}
}

static void TIMER2_Init(void){
	TIM_OC_InitTypeDef tim2PWM_Config = {0};

	/* Configure the Timer Time Base for a 1-second period */
	htimer2.Instance = TIM2;
	htimer2.Init.Period = 64000000 - 1;
	htimer2.Init.Prescaler = 0;
	htimer2.Init.CounterMode = TIM_COUNTERMODE_UP;
	htimer2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	htimer2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;

	if (HAL_TIM_PWM_Init(&htimer2) != HAL_OK){
		Error_Handler();
	}

	/* Configure common PWM parameters for all channels */
	tim2PWM_Config.OCMode = TIM_OCMODE_PWM1;
	tim2PWM_Config.OCPolarity = TIM_OCPOLARITY_HIGH;
	tim2PWM_Config.OCFastMode = TIM_OCFAST_DISABLE;

	/* Calculate and configure Pulse values (CCR registers) based on target Duty Cycles */
	/* Channel 1: 25% of 64,000,000 = 16,000,000 */
	tim2PWM_Config.Pulse = 16000000;
	if (HAL_TIM_PWM_ConfigChannel(&htimer2, &tim2PWM_Config, TIM_CHANNEL_1) != HAL_OK){
		Error_Handler();
	}

	/* Channel 2: 45% of 64,000,000 = 28,800,000 */
	tim2PWM_Config.Pulse = 28800000;
	if (HAL_TIM_PWM_ConfigChannel(&htimer2, &tim2PWM_Config, TIM_CHANNEL_2) != HAL_OK){
		Error_Handler();
	}

	/* Channel 3: 75% of 64,000,000 = 48,000,000 */
	tim2PWM_Config.Pulse = 48000000;
	if (HAL_TIM_PWM_ConfigChannel(&htimer2, &tim2PWM_Config, TIM_CHANNEL_3) != HAL_OK){
		Error_Handler();
	}

	/* Channel 4: 95% of 64,000,000 = 60,800,000 */
	tim2PWM_Config.Pulse = 60800000;
	if (HAL_TIM_PWM_ConfigChannel(&htimer2, &tim2PWM_Config, TIM_CHANNEL_4) != HAL_OK){
		Error_Handler();
	}
}

static void SystemClock_PLL_64MHz_Config(void){
	RCC_OscInitTypeDef osc_init = {0};
	RCC_ClkInitTypeDef clk_init = {0};

	HAL_PWR_EnableBkUpAccess();

    osc_init.OscillatorType = RCC_OSCILLATORTYPE_HSI;
    osc_init.HSIState = RCC_HSI_ON;
    osc_init.HSIDiv = RCC_HSI_DIV1;
    osc_init.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;

	osc_init.PLL.PLLState = RCC_PLL_ON;
	osc_init.PLL.PLLSource = RCC_PLLSOURCE_HSI;
	osc_init.PLL.PLLM = RCC_PLLM_DIV1;
    osc_init.PLL.PLLN = 8;
    osc_init.PLL.PLLR = RCC_PLLR_DIV2;

	if (HAL_RCC_OscConfig(&osc_init) != HAL_OK){
		Error_Handler();
	}

	clk_init.ClockType = RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1;
	clk_init.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	clk_init.AHBCLKDivider = RCC_SYSCLK_DIV1;
	clk_init.APB1CLKDivider = RCC_HCLK_DIV1;

	if (HAL_RCC_ClockConfig(&clk_init, FLASH_LATENCY_2) != HAL_OK){
		Error_Handler();
	}
}

void Error_Handler(void){
	  /* Disable interrupts and stay here if a serious error happens */
	  __disable_irq();
	  while (1){
	  }
}
