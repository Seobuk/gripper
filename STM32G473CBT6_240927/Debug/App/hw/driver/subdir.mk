################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../App/hw/driver/adc.c \
../App/hw/driver/dac.c \
../App/hw/driver/i2c.c \
../App/hw/driver/lcd.c \
../App/hw/driver/resize.c \
../App/hw/driver/uart.c 

OBJS += \
./App/hw/driver/adc.o \
./App/hw/driver/dac.o \
./App/hw/driver/i2c.o \
./App/hw/driver/lcd.o \
./App/hw/driver/resize.o \
./App/hw/driver/uart.o 

C_DEPS += \
./App/hw/driver/adc.d \
./App/hw/driver/dac.d \
./App/hw/driver/i2c.d \
./App/hw/driver/lcd.d \
./App/hw/driver/resize.d \
./App/hw/driver/uart.d 


# Each subdirectory must supply rules for building sources it contributes
App/hw/driver/%.o App/hw/driver/%.su App/hw/driver/%.cyclo: ../App/hw/driver/%.c App/hw/driver/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32G473xx -c -I../Core/Inc -I../Drivers/STM32G4xx_HAL_Driver/Inc -I../Drivers/STM32G4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32G4xx/Include -I../Drivers/CMSIS/Include -I"C:/Users/user/Documents/git/gripper/STM32G473CBT6_240927/App" -I"C:/Users/user/Documents/git/gripper/STM32G473CBT6_240927/App/ap" -I"C:/Users/user/Documents/git/gripper/STM32G473CBT6_240927/App/bsp" -I"C:/Users/user/Documents/git/gripper/STM32G473CBT6_240927/App/common" -I"C:/Users/user/Documents/git/gripper/STM32G473CBT6_240927/App/common/core" -I"C:/Users/user/Documents/git/gripper/STM32G473CBT6_240927/App/common/hw" -I"C:/Users/user/Documents/git/gripper/STM32G473CBT6_240927/App/hw" -I"C:/Users/user/Documents/git/gripper/STM32G473CBT6_240927/App/hw/driver" -I"C:/Users/user/Documents/git/gripper/STM32G473CBT6_240927/App/common/hw/include" -I"C:/Users/user/Documents/git/gripper/STM32G473CBT6_240927/App/common/hw/include/lcd" -I"C:/Users/user/Documents/git/gripper/STM32G473CBT6_240927/App/ap/inc" -I"C:/Users/user/Documents/git/gripper/STM32G473CBT6_240927/App/ap/src" -Og -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-App-2f-hw-2f-driver

clean-App-2f-hw-2f-driver:
	-$(RM) ./App/hw/driver/adc.cyclo ./App/hw/driver/adc.d ./App/hw/driver/adc.o ./App/hw/driver/adc.su ./App/hw/driver/dac.cyclo ./App/hw/driver/dac.d ./App/hw/driver/dac.o ./App/hw/driver/dac.su ./App/hw/driver/i2c.cyclo ./App/hw/driver/i2c.d ./App/hw/driver/i2c.o ./App/hw/driver/i2c.su ./App/hw/driver/lcd.cyclo ./App/hw/driver/lcd.d ./App/hw/driver/lcd.o ./App/hw/driver/lcd.su ./App/hw/driver/resize.cyclo ./App/hw/driver/resize.d ./App/hw/driver/resize.o ./App/hw/driver/resize.su ./App/hw/driver/uart.cyclo ./App/hw/driver/uart.d ./App/hw/driver/uart.o ./App/hw/driver/uart.su

.PHONY: clean-App-2f-hw-2f-driver

