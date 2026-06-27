# STM32G0 - Multi-Channel Signal Generation using Timer Output Compare Toggle

This project demonstrates how to generate multiple independent square waves with different frequencies using a single hardware timer (**TIM2**) on an **STM32G071** microcontroller. The implementation utilizes the **Output Compare Toggle** mode combined with hardware interrupts to advance capture-compare registers seamlessly without shifting the phase or blocking the CPU.

## Theory & Implementation Bridge

The system is configured with a core system clock running at **64 MHz**. By setting the Timer Prescaler to `0`, the timer counter (`TIM2_CNT`) increments at the maximum rate of **64,000,000 ticks per second**.

### The Core Formula
To toggle a pin at a specific target frequency ($F_{target}$), the pin must toggle twice per period (once for High-to-Low, once for Low-to-High). The pulse step calculation is:

$$\text{Pulse Step} = \frac{F_{timer}}{2 \times F_{target}}$$

Based on this formula, the 4 independent channels are configured as follows:

| Channel | GPIO Pin | Target Frequency | Pulse Step (Ticks) | Toggle Interval |
|:---:|:---:|:---:|:---:|:---:|
| **CH1** | `PA0` | 500 Hz | **64,000** | 1.000 ms |
| **CH2** | `PA1` | 1 kHz | **32,000** | 0.500 ms |
| **CH3** | `PB10` | 2 kHz | **16,000** | 0.250 ms |
| **CH4** | `PB11` | 4 kHz | **8,000** | 0.125 ms |



---

## How It Works inside the Hardware

1. **Free-Running Counter:** `TIM2_CNT` counts upward continuously from `0` to `0xFFFFFFFF`.
2. **Hardware Match & Toggle:** When `TIM2_CNT` equals the value stored in a channel's compare register (`TIM2_CCRx`), the microcontroller's hardware **instantly toggles the physical pin** and triggers an interrupt. This guarantees zero jitter because the CPU timing does not affect the waveform generation.
3. **Interrupt Service Routine (ISR):** In the `HAL_TIM_OC_DelayElapsedCallback` ISR, the CPU reads the precise snapshot of when the match occurred, adds the channel-specific pulse step value, and sets the new target milestone for the next toggle.

```c
/* Example from main.c - Advancing the target forward in time */
if (htim->Channel == HAL_TIM_ACTIVE_CHANNEL_1) {
    ccr_current = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_1);
    __HAL_TIM_SET_COMPARE(htim, TIM_CHANNEL_1, (ccr_current + pulse1_value));
}

## Logic Analyzer Verification

Below is the live capture taken from a Saleae Logic Analyzer validating the outputs:

![TIM2 Output Compare Waveforms](output_compare_toggle_waves.png)

### Analyzing the Waveform Capture:
* **Channel 0 (`PA0` - 500 Hz):** As shown on the timeline, a single toggle interval (High or Low state) lasts exactly **1 ms** (matching perfectly between the `320 ms` mark and the next `+1 ms` transition line), resulting in a full 2 ms period ($500\text{ Hz}$).
* **Frequency Scaling:** You can visually confirm that each descending channel is exactly **twice as fast** as the channel above it ($1:2:4:8$ ratio), perfectly tracking the $500\text{ Hz}$, $1\text{ kHz}$, $2\text{ kHz}$, and $4\text{ kHz}$ targets.
* **Zero CPU Overhead:** The `while(1)` loop in `main()` is entirely empty. The CPU remains completely free for other application tasks while the hardware peripheral generates pristine, highly accurate waveforms.

---

## Peripheral Configuration Specs

* **Microcontroller:** STM32G071 (ARM Cortex-M0+ Architecture)
* **Timer Peripheral:** TIM2 (32-bit Internal Timer Counter)
* **Clock Source:** HSI (Internal High-Speed RC Oscillator) via PLL @ 64 MHz
* **Toolchain:** STM32CubeIDE & HAL Drivers