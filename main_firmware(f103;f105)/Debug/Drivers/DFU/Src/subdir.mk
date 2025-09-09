################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/DFU/Src/usbd_dfu.c 

OBJS += \
./Drivers/DFU/Src/usbd_dfu.o 

C_DEPS += \
./Drivers/DFU/Src/usbd_dfu.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/DFU/Src/%.o Drivers/DFU/Src/%.su Drivers/DFU/Src/%.cyclo: ../Drivers/DFU/Src/%.c Drivers/DFU/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F105xC -c -I../Core/Inc -I"C:/Users/tomer/STM32CubeIDE/workspace_1.17.0/keyboard/Drivers/DFU/Inc" -I"C:/Users/tomer/STM32CubeIDE/workspace_1.17.0/keyboard/Drivers/CustomHID/Inc" -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -I"C:/Users/tomer/STM32CubeIDE/workspace_1.17.0/keyboard/Drivers/USB_DEVICE/App" -I"C:/Users/tomer/STM32CubeIDE/workspace_1.17.0/keyboard/Drivers/USB_DEVICE/Target" -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM3 -I../Middlewares/ST/STM32_USB_Device_Library/Core/Inc -I../USB_DEVICE/App -I../USB_DEVICE/Target -I../Middlewares/ST/STM32_USB_Device_Library/Class/CustomHID/Inc -O3 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Drivers-2f-DFU-2f-Src

clean-Drivers-2f-DFU-2f-Src:
	-$(RM) ./Drivers/DFU/Src/usbd_dfu.cyclo ./Drivers/DFU/Src/usbd_dfu.d ./Drivers/DFU/Src/usbd_dfu.o ./Drivers/DFU/Src/usbd_dfu.su

.PHONY: clean-Drivers-2f-DFU-2f-Src

