################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/CustomHID/Src/usbd_customhid.c 

OBJS += \
./Drivers/CustomHID/Src/usbd_customhid.o 

C_DEPS += \
./Drivers/CustomHID/Src/usbd_customhid.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/CustomHID/Src/%.o Drivers/CustomHID/Src/%.su Drivers/CustomHID/Src/%.cyclo: ../Drivers/CustomHID/Src/%.c Drivers/CustomHID/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32L475xx -c -I../Core/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I../Drivers/CMSIS/Include -I"C:/Users/tomer/STM32CubeIDE/workspace_1.17.0/keybored_l475rc/Drivers/CustomHID" -I"C:/Users/tomer/STM32CubeIDE/workspace_1.17.0/keybored_l475rc/Drivers/CustomHID/Inc" -I../USB_DEVICE/App -I../USB_DEVICE/Target -I../Middlewares/ST/STM32_USB_Device_Library/Core/Inc -I../Middlewares/ST/STM32_USB_Device_Library/Class/CustomHID/Inc -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Drivers-2f-CustomHID-2f-Src

clean-Drivers-2f-CustomHID-2f-Src:
	-$(RM) ./Drivers/CustomHID/Src/usbd_customhid.cyclo ./Drivers/CustomHID/Src/usbd_customhid.d ./Drivers/CustomHID/Src/usbd_customhid.o ./Drivers/CustomHID/Src/usbd_customhid.su

.PHONY: clean-Drivers-2f-CustomHID-2f-Src

