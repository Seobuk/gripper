################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../App/ap/ap.c \
../App/ap/control.c 

OBJS += \
./App/ap/ap.o \
./App/ap/control.o 

C_DEPS += \
./App/ap/ap.d \
./App/ap/control.d 


# Each subdirectory must supply rules for building sources it contributes
App/ap/%.o App/ap/%.su App/ap/%.cyclo: ../App/ap/%.c App/ap/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32G473xx -c -I../Core/Inc -I../Drivers/STM32G4xx_HAL_Driver/Inc -I../Drivers/STM32G4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32G4xx/Include -I../Drivers/CMSIS/Include -I"C:/Users/user/STM32CubeIDE/workspace/1/STM32G473CBT6/App" -I"C:/Users/user/STM32CubeIDE/workspace/1/STM32G473CBT6/App/ap" -I"C:/Users/user/STM32CubeIDE/workspace/1/STM32G473CBT6/App/bsp" -I"C:/Users/user/STM32CubeIDE/workspace/1/STM32G473CBT6/App/common" -I"C:/Users/user/STM32CubeIDE/workspace/1/STM32G473CBT6/App/common/core" -I"C:/Users/user/STM32CubeIDE/workspace/1/STM32G473CBT6/App/common/hw" -I"C:/Users/user/STM32CubeIDE/workspace/1/STM32G473CBT6/App/hw" -I"C:/Users/user/STM32CubeIDE/workspace/1/STM32G473CBT6/App/hw/driver" -I"C:/Users/user/STM32CubeIDE/workspace/1/STM32G473CBT6/App/common/hw/include" -I"C:/Users/user/STM32CubeIDE/workspace/1/STM32G473CBT6/App/common/hw/include/lcd" -Og -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-App-2f-ap

clean-App-2f-ap:
	-$(RM) ./App/ap/ap.cyclo ./App/ap/ap.d ./App/ap/ap.o ./App/ap/ap.su ./App/ap/control.cyclo ./App/ap/control.d ./App/ap/control.o ./App/ap/control.su

.PHONY: clean-App-2f-ap

