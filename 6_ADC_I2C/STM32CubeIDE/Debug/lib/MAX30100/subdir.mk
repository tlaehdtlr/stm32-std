################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../lib/MAX30100/max30100_for_stm32_hal.c 

OBJS += \
./lib/MAX30100/max30100_for_stm32_hal.o 

C_DEPS += \
./lib/MAX30100/max30100_for_stm32_hal.d 


# Each subdirectory must supply rules for building sources it contributes
lib/MAX30100/max30100_for_stm32_hal.o: ../lib/MAX30100/max30100_for_stm32_hal.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32L412xx -DDEBUG -c -I../../Core/Inc -I../../Drivers/STM32L4xx_HAL_Driver/Inc -I../../Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -I../../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I../../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"lib/MAX30100/max30100_for_stm32_hal.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

