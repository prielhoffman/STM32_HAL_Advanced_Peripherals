#include "main.h"
#include <stdio.h>

/* Function prototype */
static void SystemClock_PLL_64MHz_Config(void);
static void TIMER2_Init(void);

/*
 * Global variables for Output Compare pulse steps (Calculated for 64MHz Clock)
 * Pulse = Timer Clock Frequency / (2 * Toggle Frequency)
 */
uint32_t pulse1_value = 64000; // Generates 500Hz wave (1000Hz toggle rate)
uint32_t pulse2_value = 32000; // Generates 1kHz wave (2000Hz toggle rate)
uint32_t pulse3_value = 16000; // Generates 2kHz wave (4000Hz toggle rate)
uint32_t pulse4_value = 8000;  // Generates 4kHz wave (8000Hz toggle rate)


/* Global peripheral handle for Timer 2 */
TIM_HandleTypeDef htimer2;

int main(void){
	HAL_Init();
	SystemClock_PLL_64MHz_Config();

	TIMER2_Init();

	/* Start Output Compare generation in Interrupt mode for all 4 channels */
	if (HAL_TIM_OC_Start_IT(&htimer2, TIM_CHANNEL_1) != HAL_OK){
		Error_Handler();
	}
	if (HAL_TIM_OC_Start_IT(&htimer2, TIM_CHANNEL_2) != HAL_OK){
		Error_Handler();
	}
	if (HAL_TIM_OC_Start_IT(&htimer2, TIM_CHANNEL_3) != HAL_OK){
		Error_Handler();
	}
	if (HAL_TIM_OC_Start_IT(&htimer2, TIM_CHANNEL_4) != HAL_OK){
		Error_Handler();
	}

	while (1){

	}
}

static void TIMER2_Init(void){
	htimer2.Instance = TIM2;

	/* Set Prescaler to 0 to run the timer at the maximum clock speed (no division) */
	htimer2.Init.Prescaler = 0;
	/* Set Period (ARR) to the maximum 32-bit value to prevent frequent overflows */
	htimer2.Init.Period = 0xFFFFFFFF;
	htimer2.Init.CounterMode = TIM_COUNTERMODE_UP;

	if(HAL_TIM_OC_Init(&htimer2) != HAL_OK){
		Error_Handler();
	}

	/* Common Output Compare configuration */
	TIM_OC_InitTypeDef tim2OC_init = {0};
	tim2OC_init.OCMode = TIM_OCMODE_TOGGLE;
	tim2OC_init.OCPolarity = TIM_OCPOLARITY_HIGH;

	/* Configure Channel 1 (Target: 500Hz) */
	tim2OC_init.Pulse = pulse1_value;
	if (HAL_TIM_OC_ConfigChannel(&htimer2, &tim2OC_init, TIM_CHANNEL_1) != HAL_OK){
		Error_Handler();
	}

	/* Configure Channel 2 (Target: 1kHz) */
	tim2OC_init.Pulse = pulse2_value;
	if (HAL_TIM_OC_ConfigChannel(&htimer2, &tim2OC_init, TIM_CHANNEL_2) != HAL_OK){
		Error_Handler();
	}

	/* Configure Channel 3 (Target: 2kHz) */
	tim2OC_init.Pulse = pulse3_value;
	if (HAL_TIM_OC_ConfigChannel(&htimer2, &tim2OC_init, TIM_CHANNEL_3) != HAL_OK){
		Error_Handler();
	}

	/* Configure Channel 4 (Target: 4kHz) */
	tim2OC_init.Pulse = pulse4_value;
	if (HAL_TIM_OC_ConfigChannel(&htimer2, &tim2OC_init, TIM_CHANNEL_4) != HAL_OK){
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

/* Output Compare Delay Elapsed Callback - Moves the target forward in time */
void HAL_TIM_OC_DelayElapsedCallback(TIM_HandleTypeDef *htim){
	uint32_t ccr_current;

	if (htim->Channel == HAL_TIM_ACTIVE_CHANNEL_1){
		ccr_current = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_1);
		__HAL_TIM_SET_COMPARE(htim, TIM_CHANNEL_1, (ccr_current + pulse1_value));
	}

	if (htim->Channel == HAL_TIM_ACTIVE_CHANNEL_2){
		ccr_current = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_2);
		__HAL_TIM_SET_COMPARE(htim, TIM_CHANNEL_2, (ccr_current + pulse2_value));
	}

	if (htim->Channel == HAL_TIM_ACTIVE_CHANNEL_3){
		ccr_current = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_3);
		__HAL_TIM_SET_COMPARE(htim, TIM_CHANNEL_3, (ccr_current + pulse3_value));
	}

	if (htim->Channel == HAL_TIM_ACTIVE_CHANNEL_4){
		ccr_current = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_4);
		__HAL_TIM_SET_COMPARE(htim, TIM_CHANNEL_4, (ccr_current + pulse4_value));
	}
}
