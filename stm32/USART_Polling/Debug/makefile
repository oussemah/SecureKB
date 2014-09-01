################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include User/subdir.mk
-include GNU/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: USART_Polling.elf

# Tool invocations
USART_Polling.elf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: Cross GCC Linker'
	arm-none-eabi-gcc -L"/home/oussema/projects/stm32/l/StdPeriph_v1.2.0_Lib/Debug" -mthumb -mcpu=cortex-m3 -T"/home/oussema/projects/stm32/l/USART_Polling/GNU/STM32_flash.ld" -static -Wl,-cref,-u,Reset_Handler "-Wl,-Map=USART_Polling.map" -Wl,--gc-sections -Wl,--defsym=malloc_getpagesize_P=0x1000 -o "USART_Polling.elf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(OBJS)$(C_DEPS)$(EXECUTABLES) USART_Polling.elf
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
