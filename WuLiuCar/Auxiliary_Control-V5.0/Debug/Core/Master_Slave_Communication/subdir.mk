################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Master_Slave_Communication/Master_Slave_Communication.c 

OBJS += \
./Core/Master_Slave_Communication/Master_Slave_Communication.o 

C_DEPS += \
./Core/Master_Slave_Communication/Master_Slave_Communication.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Master_Slave_Communication/%.o Core/Master_Slave_Communication/%.su Core/Master_Slave_Communication/%.cyclo: ../Core/Master_Slave_Communication/%.c Core/Master_Slave_Communication/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xB -c -I../Core/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -I"D:/PersonalDada/Project/WuliuCar/Program/STM32/Auxiliary_Control-V5.0/Core/BLE" -I"D:/PersonalDada/Project/WuliuCar/Program/STM32/Auxiliary_Control-V5.0/Core/IMU" -I"D:/PersonalDada/Project/WuliuCar/Program/STM32/Auxiliary_Control-V5.0/Core/Tracking" -I"D:/PersonalDada/Project/WuliuCar/Program/STM32/Auxiliary_Control-V5.0/Core/Encoder" -I"D:/PersonalDada/Project/WuliuCar/Program/STM32/Auxiliary_Control-V5.0/Core/Master_Slave_Communication" -I"D:/PersonalDada/Project/WuliuCar/Program/STM32/Auxiliary_Control-V5.0/Core/Battery" -Ofast -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Core-2f-Master_Slave_Communication

clean-Core-2f-Master_Slave_Communication:
	-$(RM) ./Core/Master_Slave_Communication/Master_Slave_Communication.cyclo ./Core/Master_Slave_Communication/Master_Slave_Communication.d ./Core/Master_Slave_Communication/Master_Slave_Communication.o ./Core/Master_Slave_Communication/Master_Slave_Communication.su

.PHONY: clean-Core-2f-Master_Slave_Communication

