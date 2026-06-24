#include "main.h"

/* Function prototype */
static void GPIO_Init(void);
static void SystemClock_PLL_64MHz_Config(void);
static void TIMER2_Init(void);
static void LSE_Configuration(void);

/* Global variables for Input Capture measurement */
uint32_t input_captures[2] = {0}; /* Stores N1 (index 0) and N2 (index 1) */
uint8_t capture_count = 0; /* Tracks if it's the 1st capture or 2nd capture */
uint8_t is_capture_done = 0; /* Flag: 1 means we have a complete pair, 0 means busy/waiting */

/* Global peripheral handle for Timer 2 */
TIM_HandleTypeDef htimer2;

int main(void){
	/* Variables for calculation result analysis */
	uint32_t capture_difference = 0;
	double timer2_cnt_freq = 0;
	double timer2_cnt_res = 0;
	double user_signal_time_period = 0;
	double user_signal_freq = 0;

	HAL_Init();
	SystemClock_PLL_64MHz_Config();

	GPIO_Init();
	TIMER2_Init();
	LSE_Configuration();

	if (HAL_TIM_IC_Start_IT(&htimer2, TIM_CHANNEL_1) != HAL_OK){
		Error_Handler();
	}

	while (1){
		if (is_capture_done == 1){
		/* Check if timer count wrapped around (overflowed) between captures */
			if (input_captures[1] >= input_captures[0]){
			/* Normal case: simply subtract N1 from N2 */
				capture_difference = input_captures[1] - input_captures[0];
			}
			else{
			/* Overflow case: calculate ticks until 0xFFFFFFFF, then add the rest */
				capture_difference = (0xFFFFFFFF - input_captures[0]) + input_captures[1];
			}
			/* Timer 2 counting frequency: (PCLK1 frequency) / (Prescaler + 1) */
			timer2_cnt_freq = (double)HAL_RCC_GetPCLK1Freq() / (htimer2.Init.Prescaler + 1);
			/* Resolution: the time duration of a single tick (1 / frequency) */
			timer2_cnt_res = 1.0 / timer2_cnt_freq;
			/* Total time period of the external signal in seconds */
			user_signal_time_period = capture_difference * timer2_cnt_res;
			/* Frequency is 1 / Time Period (result in Hz) */
			user_signal_freq = 1.0 / user_signal_time_period;
			/* Reset the flag to unlock the Callback for the next capture sequence */
			is_capture_done = 0;
		}
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
	/* Set Prescaler to 0 to run the timer at the maximum clock speed (no division) */
	htimer2.Init.Prescaler = 0;
	/* Set Period (ARR) to the maximum 32-bit value to prevent frequent overflows */
	htimer2.Init.Period = 0xFFFFFFFF;
	htimer2.Init.CounterMode = TIM_COUNTERMODE_UP;

	if(HAL_TIM_IC_Init(&htimer2) != HAL_OK){
		Error_Handler();
	}

	TIM_IC_InitTypeDef tim2IC_Config = {0};

	/* Set filter to 0 (no digital noise filtering needed for clean clock signals) */
	tim2IC_Config.ICFilter = 0;
	/* Trigger a capture event on the rising edge of the input signal */
	tim2IC_Config.ICPolarity = TIM_ICPOLARITY_RISING;
	/* Map the capture channel 1 directly to the timer's input mapped pin (TI1) */
	tim2IC_Config.ICSelection = TIM_ICSELECTION_DIRECTTI;
	/* Capture every single valid rising edge (no division) */
	tim2IC_Config.ICPrescaler = TIM_ICPSC_DIV1;

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

void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim){
	if (htim->Instance == TIM2){
		/* Process only if the previous capture was handled */
		if (is_capture_done == 0){
			if (capture_count == 0){
				/* First rising edge detected: save counter value */
				input_captures[0] = HAL_TIM_GetChannelState(htim, TIM_CHANNEL_1);
				capture_count++;
			}
			else if (capture_count == 1){
				/* Second rising edge detected: save counter value */
				input_captures[1] = HAL_TIM_GetChannelState(htim, TIM_CHANNEL_1);
				capture_count = 0;
				is_capture_done = 1;
			}
		}
	}
}

static void LSE_Configuration(void){
    // Output the LSE clock onto MCO1 pin (PA8) with no division
	HAL_RCC_MCOConfig(RCC_MCO1, RCC_MCOSOURCE_LSE, RCC_MCO_DIV1);
}
