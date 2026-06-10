################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (14.3.rel1)
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include Drivers/STM32G0xx_HAL_Driver/Src/subdir.mk
-include Core/Startup/subdir.mk
-include Core/Src/subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

OPTIONAL_TOOL_DEPS := \
$(wildcard ../makefile.defs) \
$(wildcard ../makefile.init) \
$(wildcard ../makefile.targets) \


BUILD_ARTIFACT_NAME := 04_HAL_UART_RX_IT_Uppercase
BUILD_ARTIFACT_EXTENSION := elf
BUILD_ARTIFACT_PREFIX :=
BUILD_ARTIFACT := $(BUILD_ARTIFACT_PREFIX)$(BUILD_ARTIFACT_NAME)$(if $(BUILD_ARTIFACT_EXTENSION),.$(BUILD_ARTIFACT_EXTENSION),)

# Add inputs and outputs from these tool invocations to the build variables 
EXECUTABLES += \
04_HAL_UART_RX_IT_Uppercase.elf \

MAP_FILES += \
04_HAL_UART_RX_IT_Uppercase.map \

SIZE_OUTPUT += \
default.size.stdout \

OBJDUMP_LIST += \
04_HAL_UART_RX_IT_Uppercase.list \


# All Target
all: main-build

# Main-build Target
main-build: 04_HAL_UART_RX_IT_Uppercase.elf secondary-outputs

# Tool invocations
04_HAL_UART_RX_IT_Uppercase.elf 04_HAL_UART_RX_IT_Uppercase.map: $(OBJS) $(USER_OBJS) C:\Users\priel\OneDrive\Desktop\STM32_HAL_Timers_CAN_RTC_Course\04_HAL_UART_RX_IT_Uppercase\STM32G071RBTX_FLASH.ld makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-gcc -o "04_HAL_UART_RX_IT_Uppercase.elf" @"objects.list" $(USER_OBJS) $(LIBS) -mcpu=cortex-m0plus -T"C:\Users\priel\OneDrive\Desktop\STM32_HAL_Timers_CAN_RTC_Course\04_HAL_UART_RX_IT_Uppercase\STM32G071RBTX_FLASH.ld" --specs=nosys.specs -Wl,-Map="04_HAL_UART_RX_IT_Uppercase.map" -Wl,--gc-sections -static --specs=nano.specs -mfloat-abi=soft -mthumb -Wl,--start-group -lc -lm -Wl,--end-group
	@echo 'Finished building target: $@'
	@echo ' '

default.size.stdout: $(EXECUTABLES) makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-size  $(EXECUTABLES)
	@echo 'Finished building: $@'
	@echo ' '

04_HAL_UART_RX_IT_Uppercase.list: $(EXECUTABLES) makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-objdump -h -S $(EXECUTABLES) > "04_HAL_UART_RX_IT_Uppercase.list"
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) 04_HAL_UART_RX_IT_Uppercase.elf 04_HAL_UART_RX_IT_Uppercase.list 04_HAL_UART_RX_IT_Uppercase.map default.size.stdout
	-@echo ' '

secondary-outputs: $(SIZE_OUTPUT) $(OBJDUMP_LIST)

fail-specified-linker-script-missing:
	@echo 'Error: Cannot find the specified linker script. Check the linker settings in the build configuration.'
	@exit 2

warn-no-linker-script-specified:
	@echo 'Warning: No linker script specified. Check the linker settings in the build configuration.'

.PHONY: all clean dependents main-build fail-specified-linker-script-missing warn-no-linker-script-specified

-include ../makefile.targets
