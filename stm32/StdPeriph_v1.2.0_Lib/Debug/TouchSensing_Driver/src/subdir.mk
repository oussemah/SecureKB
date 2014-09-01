################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../TouchSensing_Driver/src/stm32_tsl_api.c \
../TouchSensing_Driver/src/stm32_tsl_multichannelkey.c \
../TouchSensing_Driver/src/stm32_tsl_services.c \
../TouchSensing_Driver/src/stm32_tsl_singlechannelkey.c \
../TouchSensing_Driver/src/stm32_tsl_timebase.c \
../TouchSensing_Driver/src/stm32l15x_tsl_ct_acquisition.c 

OBJS += \
./TouchSensing_Driver/src/stm32_tsl_api.o \
./TouchSensing_Driver/src/stm32_tsl_multichannelkey.o \
./TouchSensing_Driver/src/stm32_tsl_services.o \
./TouchSensing_Driver/src/stm32_tsl_singlechannelkey.o \
./TouchSensing_Driver/src/stm32_tsl_timebase.o \
./TouchSensing_Driver/src/stm32l15x_tsl_ct_acquisition.o 

C_DEPS += \
./TouchSensing_Driver/src/stm32_tsl_api.d \
./TouchSensing_Driver/src/stm32_tsl_multichannelkey.d \
./TouchSensing_Driver/src/stm32_tsl_services.d \
./TouchSensing_Driver/src/stm32_tsl_singlechannelkey.d \
./TouchSensing_Driver/src/stm32_tsl_timebase.d \
./TouchSensing_Driver/src/stm32l15x_tsl_ct_acquisition.d 


# Each subdirectory must supply rules for building sources it contributes
TouchSensing_Driver/src/%.o: ../TouchSensing_Driver/src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	arm-none-eabi-gcc -DUSE_STDPERIPH_DRIVER -DSTM32L1XX_MD -DUSE_STM32L152_EVA -DSTM32L15XXB_128K -I"/home/oussema/projects/stm32/l/StdPeriph_v1.2.0_Lib" -I"/home/oussema/projects/stm32/l/StdPeriph_v1.2.0_Lib/Board" -I"/home/oussema/projects/stm32/l/StdPeriph_v1.2.0_Lib/Utilities/STM32L152EVAL" -I"/home/oussema/projects/stm32/l/StdPeriph_v1.2.0_Lib/Utilities/Common" -I"/home/oussema/projects/stm32/l/StdPeriph_v1.2.0_Lib/StdPeriph_Driver/inc" -I"/home/oussema/projects/stm32/l/StdPeriph_v1.2.0_Lib/TouchSensing_Driver/inc" -I"/home/oussema/projects/stm32/l/StdPeriph_v1.2.0_Lib/CMSIS/ST/STM32L1xx/Include" -I"/home/oussema/projects/stm32/l/StdPeriph_v1.2.0_Lib/CMSIS/Include" -O0 -g3 -DWARF2 -Wall -c -fmessage-length=0 -mthumb -mcpu=cortex-m3 -O0 -ffunction-sections -fdata-sections  -Wall -Os -w -mlong-calls -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


