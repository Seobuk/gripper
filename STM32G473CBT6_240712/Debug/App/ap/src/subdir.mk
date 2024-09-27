################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../App/ap/src/ap.c \
../App/ap/src/control.c \
../App/ap/src/monitor.c \
../App/ap/src/sequence.c 

OBJS += \
./App/ap/src/ap.o \
./App/ap/src/control.o \
./App/ap/src/monitor.o \
./App/ap/src/sequence.o 

C_DEPS += \
./App/ap/src/ap.d \
./App/ap/src/control.d \
./App/ap/src/monitor.d \
./App/ap/src/sequence.d 


# Each subdirectory must supply rules for building sources it contributes
App/ap/src/%.o App/ap/src/%.su App/ap/src/%.cyclo: ../App/ap/src/%.c App/ap/src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32G473xx -c -I../Core/Inc -I../Drivers/STM32G4xx_HAL_Driver/Inc -I../Drivers/STM32G4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32G4xx/Include -I../Drivers/CMSIS/Include -I"C:/Users/user/STM32CubeIDE/workspace/1/STM32G473CBT6/App" -I"C:/Users/user/STM32CubeIDE/workspace/1/STM32G473CBT6/App/ap" -I"C:/Users/user/STM32CubeIDE/workspace/1/STM32G473CBT6/App/bsp" -I"C:/Users/user/STM32CubeIDE/workspace/1/STM32G473CBT6/App/common" -I"C:/Users/user/STM32CubeIDE/workspace/1/STM32G473CBT6/App/common/core" -I"C:/Users/user/STM32CubeIDE/workspace/1/STM32G473CBT6/App/common/hw" -I"C:/Users/user/STM32CubeIDE/workspace/1/STM32G473CBT6/App/hw" -I"C:/Users/user/STM32CubeIDE/workspace/1/STM32G473CBT6/App/hw/driver" -I"C:/Users/user/STM32CubeIDE/workspace/1/STM32G473CBT6/App/common/hw/include" -I"C:/Users/user/STM32CubeIDE/workspace/1/STM32G473CBT6/App/common/hw/include/lcd" -I"C:/Users/user/STM32CubeIDE/workspace/1/STM32G473CBT6/App/ap/inc" -I"C:/Users/user/STM32CubeIDE/workspace/1/STM32G473CBT6/App/ap/src" -Og -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-App-2f-ap-2f-src

clean-App-2f-ap-2f-src:
	-$(RM) ./App/ap/src/ap.cyclo ./App/ap/src/ap.d ./App/ap/src/ap.o ./App/ap/src/ap.su ./App/ap/src/control.cyclo ./App/ap/src/control.d ./App/ap/src/control.o ./App/ap/src/control.su ./App/ap/src/monitor.cyclo ./App/ap/src/monitor.d ./App/ap/src/monitor.o ./App/ap/src/monitor.su ./App/ap/src/sequence.cyclo ./App/ap/src/sequence.d ./App/ap/src/sequence.o ./App/ap/src/sequence.su

.PHONY: clean-App-2f-ap-2f-src

