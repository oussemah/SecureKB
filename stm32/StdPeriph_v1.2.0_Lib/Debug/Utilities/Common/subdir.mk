################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Utilities/Common/fonts.c \
../Utilities/Common/lcd_log.c 

OBJS += \
./Utilities/Common/fonts.o \
./Utilities/Common/lcd_log.o 

C_DEPS += \
./Utilities/Common/fonts.d \
./Utilities/Common/lcd_log.d 


# Each subdirectory must supply rules for building sources it contributes
Utilities/Common/%.o: ../Utilities/Common/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	arm-none-eabi-gcc -DUSE_STDPERIPH_DRIVER -DSTM32L1XX_MD -DUSE_STM32L152_EVA -DSTM32L15XXB_128K -I"/home/oussema/projects/stm32/l/StdPeriph_v1.2.0_Lib" -I"/home/oussema/projects/stm32/l/StdPeriph_v1.2.0_Lib/Board" -I"/home/oussema/projects/stm32/l/StdPeriph_v1.2.0_Lib/Utilities/STM32L152EVAL" -I"/home/oussema/projects/stm32/l/StdPeriph_v1.2.0_Lib/Utilities/Common" -I"/home/oussema/projects/stm32/l/StdPeriph_v1.2.0_Lib/StdPeriph_Driver/inc" -I"/home/oussema/projects/stm32/l/StdPeriph_v1.2.0_Lib/TouchSensing_Driver/inc" -I"/home/oussema/projects/stm32/l/StdPeriph_v1.2.0_Lib/CMSIS/ST/STM32L1xx/Include" -I"/home/oussema/projects/stm32/l/StdPeriph_v1.2.0_Lib/CMSIS/Include" -O0 -g3 -DWARF2 -Wall -c -fmessage-length=0 -mthumb -mcpu=cortex-m3 -O0 -ffunction-sections -fdata-sections  -Wall -Os -w -mlong-calls -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


