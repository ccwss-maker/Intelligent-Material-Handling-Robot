################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/IMU/IMU.c 

OBJS += \
./Core/IMU/IMU.o 

C_DEPS += \
./Core/IMU/IMU.d 


# Each subdirectory must supply rules for building sources it contributes
Core/IMU/%.o Core/IMU/%.su Core/IMU/%.cyclo: ../Core/IMU/%.c Core/IMU/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xB -c -I../Core/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -I"D:/PersonalDada/Project/WuliuCar/Program/STM32/Auxiliary_Control-V5.0/Core/BLE" -I"D:/PersonalDada/Project/WuliuCar/Program/STM32/Auxiliary_Control-V5.0/Core/IMU" -I"D:/PersonalDada/Project/WuliuCar/Program/STM32/Auxiliary_Control-V5.0/Core/Tracking" -I"D:/PersonalDada/Project/WuliuCar/Program/STM32/Auxiliary_Control-V5.0/Core/Encoder" -I"D:/PersonalDada/Project/WuliuCar/Program/STM32/Auxiliary_Control-V5.0/Core/Master_Slave_Communication" -I"D:/PersonalDada/Project/WuliuCar/Program/STM32/Auxiliary_Control-V5.0/Core/Battery" -Ofast -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Core-2f-IMU

clean-Core-2f-IMU:
	-$(RM) ./Core/IMU/IMU.cyclo ./Core/IMU/IMU.d ./Core/IMU/IMU.o ./Core/IMU/IMU.su

.PHONY: clean-Core-2f-IMU

