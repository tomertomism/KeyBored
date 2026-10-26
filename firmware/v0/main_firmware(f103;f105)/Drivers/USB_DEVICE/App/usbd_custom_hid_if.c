/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : usbd_custom_hid_if.c
  * @version        : v2.0_Cube
  * @brief          : USB Device Custom HID interface file.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "usbd_custom_hid_if.h"

/* USER CODE BEGIN INCLUDE */
#include "custom_keymap.h"
/* USER CODE END INCLUDE */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/

/* USER CODE BEGIN PV */
/* Private variables ---------------------------------------------------------*/
uint8_t caps_lock_status = 0;
/* USER CODE END PV */

/** @addtogroup STM32_USB_OTG_DEVICE_LIBRARY
  * @brief Usb device.
  * @{
  */

/** @addtogroup USBD_CUSTOM_HID
  * @{
  */

/** @defgroup USBD_CUSTOM_HID_Private_TypesDefinitions USBD_CUSTOM_HID_Private_TypesDefinitions
  * @brief Private types.
  * @{
  */

/* USER CODE BEGIN PRIVATE_TYPES */

/* USER CODE END PRIVATE_TYPES */

/**
  * @}
  */

/** @defgroup USBD_CUSTOM_HID_Private_Defines USBD_CUSTOM_HID_Private_Defines
  * @brief Private defines.
  * @{
  */

/* USER CODE BEGIN PRIVATE_DEFINES */

/* USER CODE END PRIVATE_DEFINES */

/**
  * @}
  */

/** @defgroup USBD_CUSTOM_HID_Private_Macros USBD_CUSTOM_HID_Private_Macros
  * @brief Private macros.
  * @{
  */

/* USER CODE BEGIN PRIVATE_MACRO */

/* USER CODE END PRIVATE_MACRO */

/**
  * @}
  */

/** @defgroup USBD_CUSTOM_HID_Private_Variables USBD_CUSTOM_HID_Private_Variables
  * @brief Private variables.
  * @{
  */

/** Usb HID report descriptor. */
__ALIGN_BEGIN static uint8_t CUSTOM_HID_ReportDesc_FS[USBD_CUSTOM_HID_REPORT_DESC_SIZE] __ALIGN_END =
{
  /* USER CODE BEGIN 0 */
	0x05, 0x01,     // Usage Page (Generic Desktop)
	0x09, 0x06,     // Usage (Keyboard)
	0xA1, 0x01,     // Collection (Application)

	0x85, 0x01,     // Report ID 1

	// 修饰键(Control, Shift, Alt等)
	0x75, 0x01,                    //   Report Size (1)
	0x95, 0x08,                    //   Report Count (8)
	0x05, 0x07,                    //   Usage Page (Key codes)
	0x19, 0xe0,                    //   Usage Minimum (Keyboard LeftControl)
	0x29, 0xe7,                    //   Usage Maxium (Keyboard Right GUI)
	0x15, 0x00,                    //   Logical Minimum (0)
	0x25, 0x01,                    //   Logical Maximum (1)
	0x81, 0x02,                    //   Input (Data, Variable, Absolute)

	// 保留字节
	0x75, 0x01,                    //   Report Size (1)
	0x95, 0x08,                    //   Report Count (8)
	0x81, 0x03,                    //   Input (Constant, Variable, Absolute)

	// LED输出报告(Num Lock, Caps Lock等)
	0x95, 0x05,                    //   Report Count (5)
	0x75, 0x01,                    //   Report Size (1)
	0x05, 0x08,                    //   Usage Page (LEDs)
	0x19, 0x01,                    //   Usage Minimum (Num Lock)
	0x29, 0x05,                    //   Usage Maxium (Reserve)
	0x91, 0x02,                    //   Output (Data, Variable, Absolute)

	0x95, 0x01,                    //   Report Count (1)
	0x75, 0x03,                    //   Report Size (3)
	0x91, 0x03,                    //   Output (Constant, Variable, Absolute)

	// 按键码(最多6个按键同时按下)
	0x95, 0x06,                    //   Report Count (6)
	0x75, 0x08,                    //   Report Size (8)
	0x15, 0x00,                    //   Logical Minimum (0)
	0x25, 0xff,                    //   Logical Maximum (1)
	0x05, 0x07,                    //   Usage Page (Key codes)
	0x19, 0x00,                    //   Usage Minimum (Reserved (no event indicated))
	0x29, 0xff,                    //   Usage Maxium (Reserved)
	0x81, 0x00,                    //   Input (Data, Array)
	0xc0,                          // End collection

	// Media Control Report (Report ID 2)
	0x05, 0x0C,                     // Usage Page (Consumer Devices)
	0x09, 0x01,                     // Usage (Consumer Control)
	0xA1, 0x01,                     // Collection (Application)

	0x85, 0x02,                     // Report ID 2

	0x95, 0x01,                     // Report Count (1)
	0x75, 0x08,                     // Report Size (8)
	0x15, 0x00,                     // Logical Minimum (0)
	0x25, 0xff,                     // Logical Maximum (1)
	0x05, 0x0c,                     // Usage Page
	0x19, 0x00,                     // Usage Minimum (Reserved (no event indicated))
	0x29, 0xFF,                     // Usage Maxium (Reserved)
	0x81, 0x00,                     // Input (Data, Ary, Abs)
  /* USER CODE END 0 */
  0xC0    /*     END_COLLECTION	             */
};

/* USER CODE BEGIN PRIVATE_VARIABLES */

/* USER CODE END PRIVATE_VARIABLES */

/**
  * @}
  */

/** @defgroup USBD_CUSTOM_HID_Exported_Variables USBD_CUSTOM_HID_Exported_Variables
  * @brief Public variables.
  * @{
  */
extern USBD_HandleTypeDef hUsbDeviceFS;

/* USER CODE BEGIN EXPORTED_VARIABLES */

/* USER CODE END EXPORTED_VARIABLES */
/**
  * @}
  */

/** @defgroup USBD_CUSTOM_HID_Private_FunctionPrototypes USBD_CUSTOM_HID_Private_FunctionPrototypes
  * @brief Private functions declaration.
  * @{
  */

static int8_t CUSTOM_HID_Init_FS(void);
static int8_t CUSTOM_HID_DeInit_FS(void);
static int8_t CUSTOM_HID_OutEvent_FS(uint8_t event_idx, uint8_t state);

/**
  * @}
  */

USBD_CUSTOM_HID_ItfTypeDef USBD_CustomHID_fops_FS =
{
  CUSTOM_HID_ReportDesc_FS,
  CUSTOM_HID_Init_FS,
  CUSTOM_HID_DeInit_FS,
  CUSTOM_HID_OutEvent_FS
};

/** @defgroup USBD_CUSTOM_HID_Private_Functions USBD_CUSTOM_HID_Private_Functions
  * @brief Private functions.
  * @{
  */

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Initializes the CUSTOM HID media low layer
  * @retval USBD_OK if all operations are OK else USBD_FAIL
  */
static int8_t CUSTOM_HID_Init_FS(void)
{
  /* USER CODE BEGIN 4 */
  return (USBD_OK);
  /* USER CODE END 4 */
}

/**
  * @brief  DeInitializes the CUSTOM HID media low layer
  * @retval USBD_OK if all operations are OK else USBD_FAIL
  */
static int8_t CUSTOM_HID_DeInit_FS(void)
{
  /* USER CODE BEGIN 5 */
  return (USBD_OK);
  /* USER CODE END 5 */
}

/**
  * @brief  Manage the CUSTOM HID class events
  * @param  event_idx: Event index
  * @param  state: Event state
  * @retval USBD_OK if all operations are OK else USBD_FAIL
  */
static int8_t CUSTOM_HID_OutEvent_FS(uint8_t event_idx, uint8_t state)
{
  /* USER CODE BEGIN 6 */
	UNUSED(event_idx);
	UNUSED(state);

	USBD_CUSTOM_HID_HandleTypeDef *hhid = (USBD_CUSTOM_HID_HandleTypeDef *)hUsbDeviceFS.pClassData;
	//uint32_t usb_recv_len = USBD_GetRxCount(&hUsbDeviceFS,CUSTOM_HID_EPIN_ADDR);

	if(hhid->Report_buf[1] == 0x02){
		caps_lock_status = 1;
		set_led_buffer_at_location(44, 0xFFFFFF);
	}else if(hhid->Report_buf[1] == 0x00){
		caps_lock_status = 0;
		uint32_t tmp = led_buffer[led_keymap[44] * 24] << 16 | led_buffer[led_keymap[44] * 24 + 1] << 8 | led_buffer[led_keymap[44] * 24 + 2];
		set_led_buffer_at_location(44, tmp);
	}
  return (USBD_OK);
  /* USER CODE END 6 */
}

/* USER CODE BEGIN 7 */
/**
  * @brief  Send the report to the Host
  * @param  report: The report to be sent
  * @param  len: The report length
  * @retval USBD_OK if all operations are OK else USBD_FAIL
  */
/*
static int8_t USBD_CUSTOM_HID_SendReport_FS(uint8_t *report, uint16_t len)
{
  return USBD_CUSTOM_HID_SendReport(&hUsbDeviceFS, report, len);
}
*/
/* USER CODE END 7 */

/* USER CODE BEGIN PRIVATE_FUNCTIONS_IMPLEMENTATION */

/* USER CODE END PRIVATE_FUNCTIONS_IMPLEMENTATION */
/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

