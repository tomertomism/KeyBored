################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/USB_DEVICE/App/usb_device.c \
../Drivers/USB_DEVICE/App/usbd_custom_hid_if.c \
../Drivers/USB_DEVICE/App/usbd_desc.c \
../Drivers/USB_DEVICE/App/usbd_dfu_if.c 

OBJS += \
./Drivers/USB_DEVICE/App/usb_device.o \
./Drivers/USB_DEVICE/App/usbd_custom_hid_if.o \
./Drivers/USB_DEVICE/App/usbd_desc.o \
./Drivers/USB_DEVICE/App/usbd_dfu_if.o 

C_DEPS += \
./Drivers/USB_DEVICE/App/usb_device.d \
./Drivers/USB_DEVICE/App/usbd_custom_hid_if.d \
./Drivers/USB_DEVICE/App/usbd_desc.d \
./Drivers/USB_DEVICE/App/usbd_dfu_if.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/USB_DEVICE/App/%.o Drivers/USB_DEVICE/App/%.su Drivers/USB_DEVICE/App/%.cyclo: ../Drivers/USB_DEVICE/App/%.c Drivers/USB_DEVICE/App/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F105xC -c -I../Core/Inc -I"C:/Users/tomer/STM32CubeIDE/workspace_1.17.0/keyboard/Drivers/DFU/Inc" -I"C:/Users/tomer/STM32CubeIDE/workspace_1.17.0/keyboard/Drivers/CustomHID/Inc" -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -I"C:/Users/tomer/STM32CubeIDE/workspace_1.17.0/keyboard/Drivers/USB_DEVICE/App" -I"C:/Users/tomer/STM32CubeIDE/workspace_1.17.0/keyboard/Drivers/USB_DEVICE/Target" -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM3 -I../Middlewares/ST/STM32_USB_Device_Library/Core/Inc -I../USB_DEVICE/App -I../USB_DEVICE/Target -I../Middlewares/ST/STM32_USB_Device_Library/Class/CustomHID/Inc -O3 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Drivers-2f-USB_DEVICE-2f-App

clean-Drivers-2f-USB_DEVICE-2f-App:
	-$(RM) ./Drivers/USB_DEVICE/App/usb_device.cyclo ./Drivers/USB_DEVICE/App/usb_device.d ./Drivers/USB_DEVICE/App/usb_device.o ./Drivers/USB_DEVICE/App/usb_device.su ./Drivers/USB_DEVICE/App/usbd_custom_hid_if.cyclo ./Drivers/USB_DEVICE/App/usbd_custom_hid_if.d ./Drivers/USB_DEVICE/App/usbd_custom_hid_if.o ./Drivers/USB_DEVICE/App/usbd_custom_hid_if.su ./Drivers/USB_DEVICE/App/usbd_desc.cyclo ./Drivers/USB_DEVICE/App/usbd_desc.d ./Drivers/USB_DEVICE/App/usbd_desc.o ./Drivers/USB_DEVICE/App/usbd_desc.su ./Drivers/USB_DEVICE/App/usbd_dfu_if.cyclo ./Drivers/USB_DEVICE/App/usbd_dfu_if.d ./Drivers/USB_DEVICE/App/usbd_dfu_if.o ./Drivers/USB_DEVICE/App/usbd_dfu_if.su

.PHONY: clean-Drivers-2f-USB_DEVICE-2f-App

