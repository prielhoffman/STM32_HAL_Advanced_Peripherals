#include "main.h"
#include <stdio.h>

/* Function prototype */
static void SystemClock_PLL_64MHz_Config(void);
static void TIMER2_Init(void);


/* Global peripheral handle for Timer 2 */
TIM_HandleTypeDef htimer2;

int main(void){
	uint16_t brightness = 0;
	uint32_t period ;

	HAL_Init();
	SystemClock_PLL_64MHz_Config();

	TIMER2_Init();

	period = htimer2.Init.Period;

	if (HAL_TIM_PWM_Start(&htimer2,TIM_CHANNEL_1) != HAL_OK){
		Error_Handler();
	}

	while (1){
		/* Phase 1: Fade Up (From 0% to 100% brightness in ~1 second) */
		while (brightness < period){
			brightness += 64;
			if (brightness > period){
				brightness = period;
			}
			__HAL_TIM_SET_COMPARE(&htimer2, TIM_CHANNEL_1, brightness);
			HAL_Delay(1);
		}
		/* Phase 2: Fade Down (From 100% to 0% brightness in ~1 second) */
		while (brightness > 0){
			if (brightness >= 64){
				brightness -= 64;
			}
			else{
				brightness = 0;
			}
			__HAL_TIM_SET_COMPARE(&htimer2, TIM_CHANNEL_1, brightness);
			HAL_Delay(1);
		}
	}
}

static void TIMER2_Init(void){
	TIM_OC_InitTypeDef tim2PWM_Config = {0};

	/* Configure the Timer Time Base for a 1-millisecond period (1 kHz) */
	htimer2.Instance = TIM2;
	htimer2.Init.Period = 64000 - 1;
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
	tim2PWM_Config.Pulse = 0;
	if (HAL_TIM_PWM_ConfigChannel(&htimer2, &tim2PWM_Config, TIM_CHANNEL_1) != HAL_OK){
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
