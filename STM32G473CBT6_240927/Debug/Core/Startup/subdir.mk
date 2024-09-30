################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
S_SRCS += \
../Core/Startup/startup_stm32g473cbtx.s 

OBJS += \
./Core/Startup/startup_stm32g473cbtx.o 

S_DEPS += \
./Core/Startup/startup_stm32g473cbtx.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Startup/%.o: ../Core/Startup/%.s Core/Startup/subdir.mk
	arm-none-eabi-gcc -mcpu=cortex-m4 -g3 -DDEBUG -c -I"C:/Users/user/Documents/git/gripper/STM32G473CBT6_240927/App" -I"C:/Users/user/Documents/git/gripper/STM32G473CBT6_240927/App/ap" -I"C:/Users/user/Documents/git/gripper/STM32G473CBT6_240927/App/bsp" -I"C:/Users/user/Documents/git/gripper/STM32G473CBT6_240927/App/common" -I"C:/Users/user/Documents/git/gripper/STM32G473CBT6_240927/App/common/core" -I"C:/Users/user/Documents/git/gripper/STM32G473CBT6_240927/App/common/hw" -I"C:/Users/user/Documents/git/gripper/STM32G473CBT6_240927/App/hw" -I"C:/Users/user/Documents/git/gripper/STM32G473CBT6_240927/App/hw/driver" -I"C:/Users/user/Documents/git/gripper/STM32G473CBT6_240927/App/common/hw/include" -I"C:/Users/user/Documents/git/gripper/STM32G473CBT6_240927/App/common/hw/include/lcd" -I"C:/Users/user/Documents/git/gripper/STM32G473CBT6_240927/App/ap/inc" -I"C:/Users/user/Documents/git/gripper/STM32G473CBT6_240927/App/ap/src" -x assembler-with-cpp -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@" "$<"

clean: clean-Core-2f-Startup

clean-Core-2f-Startup:
	-$(RM) ./Core/Startup/startup_stm32g473cbtx.d ./Core/Startup/startup_stm32g473cbtx.o

.PHONY: clean-Core-2f-Startup

