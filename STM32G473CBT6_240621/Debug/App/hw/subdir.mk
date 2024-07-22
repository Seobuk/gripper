################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../App/hw/hw.c 

OBJS += \
./App/hw/hw.o 

C_DEPS += \
./App/hw/hw.d 


# Each subdirectory must supply rules for building sources it contributes
App/hw/%.o App/hw/%.su App/hw/%.cyclo: ../App/hw/%.c App/hw/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32G473xx -c -I../Core/Inc -I../Drivers/STM32G4xx_HAL_Driver/Inc -I../Drivers/STM32G4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32G4xx/Include -I../Drivers/CMSIS/Include -I"C:/Users/user/Documents/git/gripper/STM32G473CBT6_240621/App" -I"C:/Users/user/Documents/git/gripper/STM32G473CBT6_240621/App/ap" -I"C:/Users/user/Documents/git/gripper/STM32G473CBT6_240621/App/bsp" -I"C:/Users/user/Documents/git/gripper/STM32G473CBT6_240621/App/common" -I"C:/Users/user/Documents/git/gripper/STM32G473CBT6_240621/App/common/core" -I"C:/Users/user/Documents/git/gripper/STM32G473CBT6_240621/App/common/hw" -I"C:/Users/user/Documents/git/gripper/STM32G473CBT6_240621/App/hw" -I"C:/Users/user/Documents/git/gripper/STM32G473CBT6_240621/App/hw/driver" -I"C:/Users/user/Documents/git/gripper/STM32G473CBT6_240621/App/common/hw/include" -I"C:/Users/user/Documents/git/gripper/STM32G473CBT6_240621/App/common/hw/include/lcd" -Og -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-App-2f-hw

clean-App-2f-hw:
	-$(RM) ./App/hw/hw.cyclo ./App/hw/hw.d ./App/hw/hw.o ./App/hw/hw.su

.PHONY: clean-App-2f-hw

