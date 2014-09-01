################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Utilities/STM32L152EVAL/stm32l152_eval.c \
../Utilities/STM32L152EVAL/stm32l152_eval_glass_lcd.c \
../Utilities/STM32L152EVAL/stm32l152_eval_i2c_ee.c \
../Utilities/STM32L152EVAL/stm32l152_eval_i2c_tsensor.c \
../Utilities/STM32L152EVAL/stm32l152_eval_lcd.c \
../Utilities/STM32L152EVAL/stm32l152_eval_spi_sd.c 

OBJS += \
./Utilities/STM32L152EVAL/stm32l152_eval.o \
./Utilities/STM32L152EVAL/stm32l152_eval_glass_lcd.o \
./Utilities/STM32L152EVAL/stm32l152_eval_i2c_ee.o \
./Utilities/STM32L152EVAL/stm32l152_eval_i2c_tsensor.o \
./Utilities/STM32L152EVAL/stm32l152_eval_lcd.o \
./Utilities/STM32L152EVAL/stm32l152_eval_spi_sd.o 

C_DEPS += \
./Utilities/STM32L152EVAL/stm32l152_eval.d \
./Utilities/STM32L152EVAL/stm32l152_eval_glass_lcd.d \
./Utilities/STM32L152EVAL/stm32l152_eval_i2c_ee.d \
./Utilities/STM32L152EVAL/stm32l152_eval_i2c_tsensor.d \
./Utilities/STM32L152EVAL/stm32l152_eval_lcd.d \
./Utilities/STM32L152EVAL/stm32l152_eval_spi_sd.d 


# Each subdirectory must supply rules for building sources it contributes
Utilities/STM32L152EVAL/%.o: ../Utilities/STM32L152EVAL/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	arm-none-eabi-gcc -DUSE_STDPERIPH_DRIVER -DSTM32L1XX_MD -DUSE_STM32L152_EVA -DSTM32L15XXB_128K -I"/home/oussema/projects/stm32/l/StdPeriph_v1.2.0_Lib" -I"/home/oussema/projects/stm32/l/StdPeriph_v1.2.0_Lib/Board" -I"/home/oussema/projects/stm32/l/StdPeriph_v1.2.0_Lib/Utilities/STM32L152EVAL" -I"/home/oussema/projects/stm32/l/StdPeriph_v1.2.0_Lib/Utilities/Common" -I"/home/oussema/projects/stm32/l/StdPeriph_v1.2.0_Lib/StdPeriph_Driver/inc" -I"/home/oussema/projects/stm32/l/StdPeriph_v1.2.0_Lib/TouchSensing_Driver/inc" -I"/home/oussema/projects/stm32/l/StdPeriph_v1.2.0_Lib/CMSIS/ST/STM32L1xx/Include" -I"/home/oussema/projects/stm32/l/StdPeriph_v1.2.0_Lib/CMSIS/Include" -O0 -g3 -DWARF2 -Wall -c -fmessage-length=0 -mthumb -mcpu=cortex-m3 -O0 -ffunction-sections -fdata-sections  -Wall -Os -w -mlong-calls -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


