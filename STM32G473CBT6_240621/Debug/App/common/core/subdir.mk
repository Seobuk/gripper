################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../App/common/core/qbuffer.c 

OBJS += \
./App/common/core/qbuffer.o 

C_DEPS += \
./App/common/core/qbuffer.d 


# Each subdirectory must supply rules for building sources it contributes
App/common/core/%.o App/common/core/%.su App/common/core/%.cyclo: ../App/common/core/%.c App/common/core/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32G473xx -c -I../Core/Inc -I../Drivers/STM32G4xx_HAL_Driver/Inc -I../Drivers/STM32G4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32G4xx/Include -I../Drivers/CMSIS/Include -I"C:/Users/user/Documents/git/gripper/STM32G473CBT6_240621/App" -I"C:/Users/user/Documents/git/gripper/STM32G473CBT6_240621/App/ap" -I"C:/Users/user/Documents/git/gripper/STM32G473CBT6_240621/App/bsp" -I"C:/Users/user/Documents/git/gripper/STM32G473CBT6_240621/App/common" -I"C:/Users/user/Documents/git/gripper/STM32G473CBT6_240621/App/common/core" -I"C:/Users/user/Documents/git/gripper/STM32G473CBT6_240621/App/common/hw" -I"C:/Users/user/Documents/git/gripper/STM32G473CBT6_240621/App/hw" -I"C:/Users/user/Documents/git/gripper/STM32G473CBT6_240621/App/hw/driver" -I"C:/Users/user/Documents/git/gripper/STM32G473CBT6_240621/App/common/hw/include" -I"C:/Users/user/Documents/git/gripper/STM32G473CBT6_240621/App/common/hw/include/lcd" -Og -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-App-2f-common-2f-core

clean-App-2f-common-2f-core:
	-$(RM) ./App/common/core/qbuffer.cyclo ./App/common/core/qbuffer.d ./App/common/core/qbuffer.o ./App/common/core/qbuffer.su

.PHONY: clean-App-2f-common-2f-core

