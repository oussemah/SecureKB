################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../main.c 

OBJS += \
./main.o 

C_DEPS += \
./main.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	arm-none-eabi-gcc -DUSE_STDPERIPH_DRIVER -DSTM32L1XX_MD -DUSE_STM32L152_EVAL -I"/home/oussema/projects/stm32/l/USART_Polling/User" -I"/home/oussema/projects/stm32/l/StdPeriph_v1.2.0_Lib" -I"/home/oussema/projects/stm32/l/StdPeriph_v1.2.0_Lib/Utilities/STM32L152EVAL" -I"/home/oussema/projects/stm32/l/StdPeriph_v1.2.0_Lib/Utilities/Common" -I"/home/oussema/projects/stm32/l/StdPeriph_v1.2.0_Lib/StdPeriph_Driver/inc" -I"/home/oussema/projects/stm32/l/StdPeriph_v1.2.0_Lib/Board" -I"/home/oussema/projects/stm32/l/StdPeriph_v1.2.0_Lib/CMSIS/ST/STM32L1xx/Include" -I"/home/oussema/projects/stm32/l/StdPeriph_v1.2.0_Lib/CMSIS/Include" -O0 -g3 -Wall -c -fmessage-length=0 -mthumb -mcpu=cortex-m3 -O0 -ffunction-sections -fdata-sections -g -Wall -Os -w -mlong-calls -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


