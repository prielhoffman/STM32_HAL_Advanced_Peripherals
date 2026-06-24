#include "main.h"

/* Function prototype */
static void GPIO_Init(void);
static void SystemClock_PLL_64MHz_Config(void);
static void TIMER2_Init(void);
static void LSE_Configuration(void);

/* Peripheral handles */
TIM_HandleTypeDef htimer2;

int main(void){
	  HAL_Init();
	  SystemClock_PLL_64MHz_Config();

	  GPIO_Init();
	  TIMER2_Init();
	  LSE_Configuration();

	  if (HAL_TIM_IC_Start_IT(&htimer2, TIM_CHANNEL_1) != HAL_OK){
	      Error_Handler();
	  }

	  while (1){
	  }
}

static void GPIO_Init(void){
	GPIO_InitTypeDef GPIO_InitStruct = {0};

	__HAL_RCC_GPIOA_CLK_ENABLE();

	GPIO_InitStruct.Pin = GPIO_PIN_5;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;

	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
}

static void TIMER2_Init(void){
	htimer2.Instance = TIM2;
	htimer2.Init.Prescaler = 0;
	htimer2.Init.Period = 0xFFFFFFFF;
	htimer2.Init.CounterMode = TIM_COUNTERMODE_UP;

	if(HAL_TIM_IC_Init(&htimer2) != HAL_OK){
		Error_Handler();
	}

	TIM_IC_InitTypeDef tim2IC_Config = {0};

	tim2IC_Config.ICPolarity = TIM_ICPOLARITY_RISING;
	tim2IC_Config.ICSelection = TIM_ICSELECTION_DIRECTTI;
	tim2IC_Config.ICPrescaler = TIM_ICPSC_DIV1;
	tim2IC_Config.ICFilter = 0;

	if (HAL_TIM_IC_ConfigChannel(&htimer2, &tim2IC_Config, TIM_CHANNEL_1) != HAL_OK){
	Error_Handler();
	}
}

static void SystemClock_PLL_64MHz_Config(void){
	RCC_OscInitTypeDef osc_init = {0};
	RCC_ClkInitTypeDef clk_init = {0};

	HAL_PWR_EnableBkUpAccess();

	// Update the oscillator configuration to enable both HSI and LSE
    osc_init.OscillatorType = RCC_OSCILLATORTYPE_HSI| RCC_OSCILLATORTYPE_LSE;
    osc_init.HSIState = RCC_HSI_ON;
    osc_init.HSIDiv = RCC_HSI_DIV1;
    osc_init.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;

    // Turn on the Low Speed External (LSE) crystal
	osc_init.LSEState = RCC_LSE_ON;

	osc_init.PLL.PLLState = RCC_PLL_ON;
	osc_init.PLL.PLLSource = RCC_PLLSOURCE_HSI;
	osc_init.PLL.PLLM = RCC_PLLM_DIV1;
    osc_init.PLL.PLLN = 8;
    osc_init.PLL.PLLR = RCC_PLLR_DIV2;

	if (HAL_RCC_OscConfig(&osc_init) != HAL_OK){
		Error_Handler();
	}

    /*
     * Select PLLCLK as SYSCLK.
     *
     * SYSCLK = 64 MHz
     * HCLK   = SYSCLK / 1 = 64 MHz
     * PCLK1  = HCLK / 1   = 64 MHz
     */
	clk_init.ClockType = RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1;
	clk_init.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	clk_init.AHBCLKDivider = RCC_SYSCLK_DIV1;
	clk_init.APB1CLKDivider = RCC_HCLK_DIV1;

    /* HCLK is only 4 MHz, so 0 wait states are enough for Flash */
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

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim){
	if (htim->Instance == TIM6){
        /*
         * This callback is called every 100 ms,
         * because TIM6 was configured with PSC = 63999 and ARR = 99.
         */
		HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
	}
}

static void LSE_Configuration(void){
    // Output the LSE clock onto MCO1 pin (PA8) with no division
	HAL_RCC_MCOConfig(RCC_MCO1, RCC_MCOSOURCE_LSE, RCC_MCO_DIV1);
}
