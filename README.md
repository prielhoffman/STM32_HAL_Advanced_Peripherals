# STM32G0 Advanced Peripherals & Hardware Verification

Welcome to my bare-metal and HAL development repository for the **STM32G071** microcontroller (ARM Cortex-M0+ architecture). This repository serves as a structured, hands-on portfolio demonstrating lower-level peripheral configuration, timing analysis, and rigorous hardware verification using a digital Logic Analyzer.

The primary objective of these projects is to build robust embedded systems applications while emphasizing **0% CPU overhead execution** and precise register-level timing verification.

---

## Global Hardware & System Specifications

Across all sub-projects in this repository, the core clock tree and environmental setups are strictly configured as follows:
* **Microcontroller:** STM32G071RBTx (NUCLEO-G071RB Development Board).
* **System Clock (SYSCLK):** Driven to its maximum stable internal frequency of **64 MHz** by utilizing the High-Speed Internal Oscillator (HSI @ 16 MHz) combined with the internal **PLL** loop ($16\text{ MHz} \times 8 / 2 = 64\text{ MHz}$).
* **Toolchain & Environment:** STM32CubeIDE, ST-LINK V2, and pure C HAL Drivers combined with explicit macro/register manipulations.
* **Hardware Verification:** All output timings, duty cycles, and signal phases are captured and validated via a **Saleae Logic Analyzer** running at sampling rates up to 12 MS/s.

---

## Repository Structure & Project Roadmap

Here is a breakdown of the specific peripheral applications implemented in this repository:

###  UART Peripherals
* **01_HAL_UART_HelloWorld:** Core initialization of the Universal Asynchronous Receiver-Transmitter. Basic polling transmission.
* **02_HAL_UART_Uppercase_Echo:** Polling-based echo mechanism with runtime character transformation.
* **03_HAL_UART_Manual_Init_Fl...** Deep dive into setting up custom hardware initialization structures outside standard code generation boundaries.
* **04_HAL_UART_RX_IT_Upperc...** Transition to non-blocking architectures. Implements UART reception using **Hardware Interrupts (ISR)** for responsive asynchronous string parsing.

### Clock Tree & Core Timing
* **05_HSE_SYSCLK_8Mhz:** Custom clock configuration routing the external crystal oscillator directly to the system core.
* **06_PLL_SYSCLK_64MHz:** Implementation of the main high-speed 64 MHz PLL framework used throughout advanced exercises.
* **07_TIM6_TimeBase_100ms:** Basic 16-bit basic timer setup to generate exact 100 ms periodic system ticks.

### Advanced Timers & Signal Generation

#### 08_TIM2_InputCapture_1
Configuring standard input channels to capture physical external pulse widths and measure hardware intervals on the fly.

#### 09_TIM2_OutputCompare_Toggle
* **Concept:** Generates 4 completely independent square waves using a single 32-bit hardware timer (`TIM2`).
* **Implementation:** Leverages Output Compare Toggle mode coupled with an ISR (`HAL_TIM_OC_DelayElapsedCallback`) to shift capture thresholds dynamically. 
* **Frequencies:** Generates pristine **500 Hz**, **1 kHz**, **2 kHz**, and **4 kHz** tracks simultaneously with zero jitter.

#### 10_timer_OC_PWM1
Baseline Pulse Width Modulation implementation exploring standard edge-aligned counter behaviors and discrete channel configurations.

#### 11_timer_PWM_LED_Dimming (Breathing Effect)
* **Concept:** Implements a dynamic LED breathing/fading profile via continuous hardware duty cycle modulation.
* **Implementation:** `TIM2 Channel 1` is bound to pin `PA0` running at a base PWM frequency of **1 kHz** (ARR = 63,999). 
* **Runtime Loop:** The main execution routine programmatically steps the compare register (`CCR1`) up and down between 0% and 100% duty cycles in 1 ms increments, creating a fluid, human-eye readable fading transition.

---

## Hardware Verification Methodology

Every single waveform generated in this repository is scrutinized for real-world compliance. For instance, during the Multi-Channel Output Compare execution, the physical output pins are analyzed to guarantee phase correctness and absolute CPU independence:

* **Zero CPU Bloat:** The main application `while(1)` loops are kept completely empty where possible, pushing the execution workload entirely onto the microcontroller's internal peripheral hardware.
* **Nanosecond Granularity:** By utilizing 32-bit counters (like TIM2) at 64 MHz, we maintain full clock resolution without requiring aggressive prescaler attenuation.

---

## How to Run the Projects

1. Clone this repository to your local machine.
2. Open **STM32CubeIDE** and import the desired sub-project directory.
3. Build the project using the `GNU ARM Embedded Toolchain`.
4. Flash the generated binary onto your STM32G0 board via ST-LINK.
5. Connect a Logic Analyzer to the corresponding GPIO pins mapped in `stm32g0xx_hal_msp.c` to visually trace the execution.
