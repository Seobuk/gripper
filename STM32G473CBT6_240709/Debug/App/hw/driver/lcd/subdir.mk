################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../App/hw/driver/lcd/lcd_fonts.c \
../App/hw/driver/lcd/ssd1306.c 

OBJS += \
./App/hw/driver/lcd/lcd_fonts.o \
./App/hw/driver/lcd/ssd1306.o 

C_DEPS += \
./App/hw/driver/lcd/lcd_fonts.d \
./App/hw/driver/lcd/ssd1306.d 


# Each subdirectory must supply rules for building sources it contributes
App/hw/driver/lcd/%.o App/hw/driver/lcd/%.su App/hw/driver/lcd/%.cyclo: ../App/hw/driver/lcd/%.c App/hw/driver/lcd/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32G473xx -c -I../Core/Inc -I../Drivers/STM32G4xx_HAL_Driver/Inc -I../Drivers/STM32G4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32G4xx/Include -I../Drivers/CMSIS/Include -I"C:/Users/user/Documents/git/gripper/STM32G473CBT6_240709/App" -I"C:/Users/user/Documents/git/gripper/STM32G473CBT6_240709/App/ap" -I"C:/Users/user/Documents/git/gripper/STM32G473CBT6_240709/App/bsp" -I"C:/Users/user/Documents/git/gripper/STM32G473CBT6_240709/App/common" -I"C:/Users/user/Documents/git/gripper/STM32G473CBT6_240709/App/common/core" -I"C:/Users/user/Documents/git/gripper/STM32G473CBT6_240709/App/common/hw" -I"C:/Users/user/Documents/git/gripper/STM32G473CBT6_240709/App/hw" -I"C:/Users/user/Documents/git/gripper/STM32G473CBT6_240709/App/hw/driver" -I"C:/Users/user/Documents/git/gripper/STM32G473CBT6_240709/App/common/hw/include" -I"C:/Users/user/Documents/git/gripper/STM32G473CBT6_240709/App/common/hw/include/lcd" -Og -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-App-2f-hw-2f-driver-2f-lcd

clean-App-2f-hw-2f-driver-2f-lcd:
	-$(RM) ./App/hw/driver/lcd/lcd_fonts.cyclo ./App/hw/driver/lcd/lcd_fonts.d ./App/hw/driver/lcd/lcd_fonts.o ./App/hw/driver/lcd/lcd_fonts.su ./App/hw/driver/lcd/ssd1306.cyclo ./App/hw/driver/lcd/ssd1306.d ./App/hw/driver/lcd/ssd1306.o ./App/hw/driver/lcd/ssd1306.su

.PHONY: clean-App-2f-hw-2f-driver-2f-lcd

