################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
S_SRCS += \
../Application/User/Startup/startup_stm32f412zgtx.s 

OBJS += \
./Application/User/Startup/startup_stm32f412zgtx.o 

S_DEPS += \
./Application/User/Startup/startup_stm32f412zgtx.d 


# Each subdirectory must supply rules for building sources it contributes
Application/User/Startup/startup_stm32f412zgtx.o: ../Application/User/Startup/startup_stm32f412zgtx.s
	arm-none-eabi-gcc -mcpu=cortex-m4 -g3 -c -x assembler-with-cpp -MMD -MP -MF"Application/User/Startup/startup_stm32f412zgtx.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@" "$<"
