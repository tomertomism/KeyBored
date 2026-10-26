/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
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
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "adc.h"
#include "dma.h"
#include "iwdg.h"
//#include "spi.h"
#include "tim.h"
#include "usb_device.h"
#include "gpio.h"

//#include "usbd_hid.h"
#include "usbd_customhid.h"
#include "custom_keymap.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define Modify_pos 1
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */

uint8_t entre_DFU_mode = 0;

uint8_t usb_ready = 0;
uint8_t fn_pressed = 0;
uint8_t media_pressed = 0;
uint8_t knob_triggered = 0;
uint8_t led_change_triggered = 0;
uint8_t win_key_change_triggered = 0;
uint8_t win_key_enable = 1;
uint8_t global_buffer[CUSTOM_HID_EPIN_SIZE] = { 0x01, 0, 0, 0, 0, 0, 0, 0, 0 };
uint8_t global_media[2] = { 0x02, 0};
uint32_t led_buffer[led_buffer_length];
uint8_t led_mode = 0;
uint8_t led_mode_changed = 0;

/* USER CODE END Variables */
/* Definitions for defaultTask */
osThreadId_t defaultTaskHandle;
const osThreadAttr_t defaultTask_attributes = {
  .name = "defaultTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for encoderTask */
osThreadId_t encoderTaskHandle;
const osThreadAttr_t encoderTask_attributes = {
  .name = "encoderTask",
  .stack_size = 64 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for buttonScanTask */
osThreadId_t buttonScanTaskHandle;
const osThreadAttr_t buttonScanTask_attributes = {
  .name = "buttonScanTask",
  .stack_size = 64 * 4,
  .priority = (osPriority_t) osPriorityAboveNormal,
};
/* Definitions for ledTask */
osThreadId_t ledTaskHandle;
const osThreadAttr_t ledTask_attributes = {
  .name = "ledTask",
  .stack_size = 64 * 4,
  .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for ledModeChange */
osThreadId_t ledModeChangeHandle;
const osThreadAttr_t ledModeChange_attributes = {
  .name = "ledModeChange",
  .stack_size = 64 * 4,
  .priority = (osPriority_t) osPriorityLow,
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void *argument);
void StartencoderTask(void *argument);
void StartbuttonScanTask(void *argument);
void StartledTask(void *argument);
void StartledModeChange(void *argument);

extern void MX_USB_DEVICE_Init(void);
void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */
//  column_13_set;
//  while(!column_13_offset);
//  if(!entre_DFU_mode && row_3_offset)
//	  entre_DFU_mode = 1;
//  column_13_reset;
  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of defaultTask */
  defaultTaskHandle = osThreadNew(StartDefaultTask, NULL, &defaultTask_attributes);

  /* creation of encoderTask */
  encoderTaskHandle = osThreadNew(StartencoderTask, NULL, &encoderTask_attributes);

  /* creation of buttonScanTask */
  buttonScanTaskHandle = osThreadNew(StartbuttonScanTask, NULL, &buttonScanTask_attributes);

  /* creation of ledTask */
  ledTaskHandle = osThreadNew(StartledTask, NULL, &ledTask_attributes);

  /* creation of ledModeChange */
  ledModeChangeHandle = osThreadNew(StartledModeChange, NULL, &ledModeChange_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

}

/* USER CODE BEGIN Header_StartDefaultTask */
/**
  * @brief  Function implementing the defaultTask thread.
  * @param  argument: Not used
  * @retval None
  */
/*
  if(entre_DFU_mode == 2)
	  MX_USB_DEVICE_Init_DFU();
  else{
	  MX_USB_DEVICE_Init();
	  usb_ready = 1;
  }
*/
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void *argument)
{
  /* init code for USB_DEVICE */
  MX_USB_DEVICE_Init();
  /* USER CODE BEGIN StartDefaultTask */
  usb_ready = 1;
  osDelay(750);
  /* Infinite loop */
  for(;;)
  {
	  if(usb_ready){
		  if(media_pressed){
			  USBD_CUSTOM_HID_SendReport(&hUsbDeviceFS, global_media, 2);
			  media_pressed--;
			  knob_triggered = 0;
		  }else{
			  USBD_CUSTOM_HID_SendReport(&hUsbDeviceFS, global_buffer, CUSTOM_HID_EPIN_SIZE);
		  }
		  if(win_key_change_triggered > 0)
			  win_key_change_triggered--;
		  if(led_change_triggered > 0)
			  led_change_triggered--;
	  }
	  osDelay(CUSTOM_HID_FS_BINTERVAL);
  }
  /* USER CODE END StartDefaultTask */
}

/* USER CODE BEGIN Header_StartencoderTask */
/**
* @brief Function implementing the encoderTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartencoderTask */
void StartencoderTask(void *argument)
{
  /* USER CODE BEGIN StartencoderTask */
	while(!usb_ready)
		osDelay(100);
	osDelay(750);
	HAL_TIM_Encoder_Start(&htim3, TIM_CHANNEL_1 | TIM_CHANNEL_2);
  /* Infinite loop */
  for(;;)
  {
	  while(!TIM3->CNT){
		  osDelay(1);
	  }
	  uint8_t direction = __HAL_TIM_IS_TIM_COUNTING_DOWN(&htim3);

	  if(TIM3->CNT % 4 == 0){
		  if(direction){
			  global_media[1] = Key_MEDIA_VOLUMEUP;
		  }else{
			  global_media[1] = Key_MEDIA_VOLUMEDOWN;
		  }
		  media_pressed = 2;
		  knob_triggered = 1;
		  TIM3->CNT = 0;
	  }
	  osDelay(1);
  }
  /* USER CODE END StartencoderTask */
}

/* USER CODE BEGIN Header_StartbuttonScanTask */
/**
* @brief Function implementing the buttonScanTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartbuttonScanTask */
void StartbuttonScanTask(void *argument)
{
  /* USER CODE BEGIN StartbuttonScanTask */
	while(!usb_ready)
		osDelay(100);
	osDelay(750);
  /* Infinite loop */
  for(;;)
  {
	  uint8_t buffer[CUSTOM_HID_EPIN_SIZE] = { 0x01, 0, 0, 0, 0, 0, 0, 0, 0 };
	  uint8_t media[3] = { 0x02, 0, 0};
	  //TODO
	  if(fn_pressed)
		  fn_press_handle(buffer, media);
	  else
		  fn_not_press_handle(buffer, media);

	  //buffer[2] = Key_NONE;
	  memcpy(global_buffer, buffer, CUSTOM_HID_EPIN_SIZE);
	  if(!knob_triggered)
		  memcpy(global_media, media, 2);
	  osDelay(1);
  }
  /* USER CODE END StartbuttonScanTask */
}

/* USER CODE BEGIN Header_StartledTask */
/**
* @brief Function implementing the ledTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartledTask */
void StartledTask(void *argument)
{
  /* USER CODE BEGIN StartledTask */
	while(!usb_ready)
		osDelay(100);
	//ws2812 format GRB
	fill_led_buffer(calc_led_brightness(0xff, 0xff00ff));
  /* Infinite loop */
  for(;;)
  {
	HAL_IWDG_Refresh(&hiwdg);
	//TODO
	HAL_TIM_PWM_Start_DMA(&htim4, TIM_CHANNEL_3, (uint32_t *)led_buffer, led_buffer_length);
    osDelay(40);
  }
  /* USER CODE END StartledTask */
}

/* USER CODE BEGIN Header_StartledModeChange */
/**
* @brief Function implementing the ledModeChange thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartledModeChange */
void StartledModeChange(void *argument)
{
  /* USER CODE BEGIN StartledModeChange */
	while(!usb_ready)
		osDelay(100);
  /* Infinite loop */
  for(;;)
  {
		while(!led_change_triggered){
			osDelay(10);
		}

		if(led_mode % 8 == 0){
			fill_led_buffer(calc_led_brightness(0xff, 0xff00ff));
		}
		else if(led_mode % 8 == 1){
			fill_led_buffer(calc_led_brightness(0xff, 0xff0000));
		}
		else if(led_mode % 8 == 2){
			fill_led_buffer(calc_led_brightness(0xff, 0xffff00));
		}
		else if(led_mode % 8 == 3){
			fill_led_buffer(calc_led_brightness(0xff, 0x00ff00));
		}
		else if(led_mode % 8 == 4){
			fill_led_buffer(calc_led_brightness(0xff, 0x00ffff));
			led_mode_changed = 0;
		}
		else if(led_mode % 8 == 5){
			fill_led_buffer(calc_led_brightness(0xff, 0x0000ff));
		}
		osDelay(50);
  }
  /* USER CODE END StartledModeChange */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */
void HAL_TIM_PWM_PulseFinishedCallback(TIM_HandleTypeDef *htim)
{
	if(htim == &htim4)
		HAL_TIM_PWM_Stop(&htim4, TIM_CHANNEL_3);
}

void scan_delayus(uint16_t tick){
	while(tick--){
		uint8_t tmp = 72;
		while(tmp--)
			__NOP();
	}
}

void fn_press_handle(uint8_t * buffer, uint8_t * media){
	column_1_set;
	while(column_0_offset && !column_1_offset);
	if(row_5_offset){
		if(!win_key_change_triggered && win_key_enable){
			set_led_buffer_at_location(73, 0);
			win_key_enable = 0;
		}else if(!win_key_change_triggered && !win_key_enable){
			win_key_enable = 1;
		}
		win_key_change_triggered = 2;
	}
	column_1_reset;
	scan_delayus(column_scan_interval);

	column_7_set;
	while(column_13_offset && !column_7_offset);
	if(row_0_offset){
	  if(!media_pressed)
	  	  media[1] = keymap_fn[7];
	  media_pressed = 2;
	}
	column_7_reset;
	scan_delayus(column_scan_interval);

	column_8_set;
	while(column_14_offset && !column_8_offset);
	if(row_0_offset){
	  if(!media_pressed)
	  	  media[1] = keymap_fn[8];
	  media_pressed = 2;
	}
	column_8_reset;
	scan_delayus(column_scan_interval);

	column_9_set;
	while(column_1_offset && !column_9_offset);
	if(row_0_offset){
	  if(!media_pressed)
	  	  media[1] = keymap_fn[9];
	  media_pressed = 2;
	}
	column_9_reset;
	scan_delayus(column_scan_interval);


	column_10_set;
	while(column_3_offset && !column_10_offset);
	if(column_10_offset && row_5_offset)
	  fn_pressed = 1;
	else
	  fn_pressed = 0;
	column_10_reset;
	scan_delayus(column_scan_interval);

	column_13_set;
	while(column_6_offset && !column_13_offset);
	if(row_5_offset){
		if(!led_change_triggered)
			led_mode--;
		led_change_triggered = 2;
	}
	column_13_reset;
	scan_delayus(column_scan_interval);

	column_15_set;
	while(column_8_offset && !column_15_offset);
	if(row_5_offset){
		if(!led_change_triggered)
			led_mode++;
		led_change_triggered = 2;
	}
	column_15_reset;
}

void fn_not_press_handle(uint8_t * buffer, uint8_t * media){
	uint8_t count = Modify_pos + 2;
	column_0_set;
	while(column_15_offset && !column_0_offset);
	if(count < 8 && column_0_offset && row_0_offset)
	  buffer[count++] = keymap_1[0];
	if(count < 8 && column_0_offset && row_1_offset)
	  buffer[count++] = keymap_1[15];
	if(count < 8 && column_0_offset && row_2_offset)
	  buffer[count++] = keymap_1[30];
	if(count < 8 && column_0_offset && row_3_offset)
	  buffer[count++] = keymap_1[45];
	if(column_0_offset && row_4_offset)
	  buffer[Modify_pos] |= Modify_LShift;
	if(column_0_offset && row_5_offset)
	  buffer[Modify_pos] |= Modify_LCtrl;
	column_0_reset;
	scan_delayus(column_scan_interval);

	column_1_set;
	while(column_0_offset && !column_1_offset);
	if(count < 8 && column_1_offset && row_0_offset){
	  if(!media_pressed)
		media[1] = keymap_1[1];
	  media_pressed = 2;
	}
	if(count < 8 && column_1_offset && row_1_offset)
	  buffer[count++] = keymap_1[16];
	if(count < 8 && column_1_offset && row_2_offset)
	  buffer[count++] = keymap_1[31];
	if(count < 8 && column_1_offset && row_3_offset)
	  buffer[count++] = keymap_1[46];
	if(count < 8 && column_1_offset && row_4_offset)
	  buffer[count++] = keymap_1[60];
	if(column_1_offset && row_5_offset)
	  buffer[Modify_pos] |= (win_key_enable) ? Modify_LMeta : 0;
	column_1_reset;
	scan_delayus(column_scan_interval);

	column_2_set;
	while(column_9_offset && !column_2_offset);
	if(count < 8 && column_2_offset && row_0_offset)
	  buffer[count++] = keymap_1[2];
	if(count < 8 && column_2_offset && row_1_offset)
	  buffer[count++] = keymap_1[17];
	if(count < 8 && column_2_offset && row_2_offset)
	  buffer[count++] = keymap_1[32];
	if(count < 8 && column_2_offset && row_3_offset)
	  buffer[count++] = keymap_1[47];
	if(count < 8 && column_2_offset && row_4_offset)
	  buffer[count++] = keymap_1[61];
	if(column_2_offset && row_5_offset)
	  buffer[Modify_pos] |= Modify_LAlt;
	column_2_reset;
	scan_delayus(column_scan_interval);

	column_3_set;
	while(column_2_offset && !column_3_offset);
	if(count < 8 && column_3_offset && row_0_offset)
	  buffer[count++] = keymap_1[3];
	if(count < 8 && column_3_offset && row_1_offset)
	  buffer[count++] = keymap_1[18];
	if(count < 8 && column_3_offset && row_2_offset)
	  buffer[count++] = keymap_1[33];
	if(count < 8 && column_3_offset && row_3_offset)
	  buffer[count++] = keymap_1[48];
	if(count < 8 && column_3_offset && row_4_offset)
	  buffer[count++] = keymap_1[62];
	column_3_reset;
	scan_delayus(column_scan_interval);

	column_4_set;
	while(column_10_offset && !column_4_offset);
	if(count < 8 && column_4_offset && row_0_offset)
	  buffer[count++] = keymap_1[4];
	if(count < 8 && column_4_offset && row_1_offset)
	  buffer[count++] = keymap_1[19];
	if(count < 8 && column_4_offset && row_2_offset)
	  buffer[count++] = keymap_1[34];
	if(count < 8 && column_4_offset && row_3_offset)
	  buffer[count++] = keymap_1[49];
	if(count < 8 && column_4_offset && row_4_offset)
	  buffer[count++] = keymap_1[63];
	column_4_reset;
	scan_delayus(column_scan_interval);

	column_5_set;
	while(column_11_offset && !column_5_offset);
	if(count < 8 && column_5_offset && row_0_offset)
	  buffer[count++] = keymap_1[5];
	if(count < 8 && column_5_offset && row_1_offset)
	  buffer[count++] = keymap_1[20];
	if(count < 8 && column_5_offset && row_2_offset)
	  buffer[count++] = keymap_1[35];
	if(count < 8 && column_5_offset && row_3_offset)
	  buffer[count++] = keymap_1[50];
	if(count < 8 && column_5_offset && row_4_offset)
	  buffer[count++] = keymap_1[64];
	if(count < 8 && column_5_offset && row_5_offset)
	  buffer[count++] = keymap_1[76];
	column_5_reset;
	scan_delayus(column_scan_interval);

	column_6_set;
	while(column_12_offset && !column_6_offset);
	if(count < 8 && column_6_offset && row_0_offset)
	  buffer[count++] = keymap_1[6];
	if(count < 8 && column_6_offset && row_1_offset)
	  buffer[count++] = keymap_1[21];
	if(count < 8 && column_6_offset && row_2_offset)
	  buffer[count++] = keymap_1[36];
	if(count < 8 && column_6_offset && row_3_offset)
	  buffer[count++] = keymap_1[51];
	if(count < 8 && column_6_offset && row_4_offset)
	  buffer[count++] = keymap_1[65];
	column_6_reset;
	scan_delayus(column_scan_interval);

	column_7_set;
	while(column_13_offset && !column_7_offset);
	if(count < 8 && column_7_offset && row_0_offset)
	  buffer[count++] = keymap_1[7];
	if(count < 8 && column_7_offset && row_1_offset)
	  buffer[count++] = keymap_1[22];
	if(count < 8 && column_7_offset && row_2_offset)
	  buffer[count++] = keymap_1[37];
	if(count < 8 && column_7_offset && row_3_offset)
	  buffer[count++] = keymap_1[52];
	if(count < 8 && column_7_offset && row_4_offset)
	  buffer[count++] = keymap_1[66];
	column_7_reset;
	scan_delayus(column_scan_interval);

	column_8_set;
	while(column_14_offset && !column_8_offset);
	if(count < 8 && column_8_offset && row_0_offset)
	  buffer[count++] = keymap_1[8];
	if(count < 8 && column_8_offset && row_1_offset)
	  buffer[count++] = keymap_1[23];
	if(count < 8 && column_8_offset && row_2_offset)
	  buffer[count++] = keymap_1[38];
	if(count < 8 && column_8_offset && row_3_offset)
	  buffer[count++] = keymap_1[53];
	if(count < 8 && column_8_offset && row_4_offset)
	  buffer[count++] = keymap_1[67];
	column_8_reset;
	scan_delayus(column_scan_interval);

	column_9_set;
	while(column_1_offset && !column_9_offset);
	if(count < 8 && column_9_offset && row_0_offset)
	  buffer[count++] = keymap_1[9];
	if(count < 8 && column_9_offset && row_1_offset)
	  buffer[count++] = keymap_1[24];
	if(count < 8 && column_9_offset && row_2_offset)
	  buffer[count++] = keymap_1[39];
	if(count < 8 && column_9_offset && row_3_offset)
	  buffer[count++] = keymap_1[54];
	if(count < 8 && column_9_offset && row_4_offset)
	  buffer[count++] = keymap_1[68];
	if(column_9_offset && row_5_offset)
	  buffer[Modify_pos] |= Modify_RAlt;
	column_9_reset;
	scan_delayus(column_scan_interval);

	column_10_set;
	while(column_3_offset && !column_10_offset);
	if(count < 8 && column_10_offset && row_1_offset)
	  buffer[count++] = keymap_1[25];
	if(count < 8 && column_10_offset && row_2_offset)
	  buffer[count++] = keymap_1[40];
	if(count < 8 && column_10_offset && row_3_offset)
	  buffer[count++] = keymap_1[55];
	if(count < 8 && column_10_offset && row_4_offset)
	  buffer[count++] = keymap_1[69];
	if(column_10_offset && row_5_offset)
	  fn_pressed = 1;
	else
	  fn_pressed = 0;
	column_10_reset;
	scan_delayus(column_scan_interval);

	column_11_set;
	while(column_4_offset && !column_11_offset);
	if(count < 8 && row_0_offset)
	  buffer[count++] = keymap_1[10];
	if(count < 8 && row_1_offset)
	  buffer[count++] = keymap_1[26];
	if(count < 8 && row_2_offset)
	  buffer[count++] = keymap_1[41];
	if(count < 8 && row_3_offset)
	  buffer[count++] = keymap_1[56];
	if(row_5_offset)
	  buffer[Modify_pos] |= Modify_RCtrl;
	column_11_reset;
	scan_delayus(column_scan_interval);

	column_12_set;
	while(column_5_offset && !column_12_offset);
	if(count < 8 && row_0_offset)
	  buffer[count++] = keymap_1[11];
	if(count < 8 && row_1_offset)
	  buffer[count++] = keymap_1[27];
	if(count < 8 && row_2_offset)
	  buffer[count++] = keymap_1[42];
	if(row_4_offset)
	  buffer[Modify_pos] |= Modify_RShift;
	column_12_reset;
	scan_delayus(column_scan_interval);

	column_13_set;
	while(column_6_offset && !column_13_offset);
	if(count < 8 && row_0_offset)
	  buffer[count++] = keymap_1[12];
	if(count < 8 && row_1_offset)
	  buffer[count++] = keymap_1[28];
	if(count < 8 && row_2_offset)
	  buffer[count++] = keymap_1[43];
	if(count < 8 && row_3_offset)
	  buffer[count++] = keymap_1[57];
	if(count < 8 && row_5_offset)
	  buffer[count++] = keymap_1[80];

	column_13_reset;
	scan_delayus(column_scan_interval);

	column_14_set;
	while(column_7_offset && !column_14_offset);
	if(count < 8 && row_0_offset)
	  buffer[count++] = keymap_1[13];
	if(count < 8 && row_4_offset)
	  buffer[count++] = keymap_1[71];
	if(count < 8 && row_5_offset)
	  buffer[count++] = keymap_1[81];
	column_14_reset;
	scan_delayus(column_scan_interval);

	column_15_set;
	while(column_8_offset && !column_15_offset);
	if(count < 8 && row_0_offset)
	  buffer[count++] = keymap_1[14];
	if(count < 8 && row_1_offset)
	  buffer[count++] = keymap_1[29];
	if(count < 8 && row_2_offset)
	  buffer[count++] = keymap_1[44];
	if(count < 8 && row_3_offset)
	  buffer[count++] = keymap_1[58];
	if(count < 8 && row_4_offset)
	  buffer[count++] = keymap_1[72];
	if(count < 8 && row_5_offset)
	  buffer[count++] = keymap_1[82];
	column_15_reset;
}

void fill_led_buffer(uint32_t color_rgb){
	for(uint16_t i = 0; i < led_count; i++){
		uint8_t is_enable = 1;
		if(i == led_keymap[44] && caps_lock_status)
			is_enable = 0;
		if(i == led_keymap[73] && !win_key_enable)
			is_enable = 0;
		if(is_enable){
			for(uint8_t j = 0; j < 8; j++){
				led_buffer[(i * 24) +       j]   = (color_rgb & (1 << (15 - j))) ? LED_LOGIC_1 : LED_LOGIC_0;// G channel
				led_buffer[(i * 24) + ( 8 + j)]  = (color_rgb & (1 << (23 - j))) ? LED_LOGIC_1 : LED_LOGIC_0;// R channel
				led_buffer[(i * 24) + (16 + j)]  = (color_rgb & (1 << ( 7 - j))) ? LED_LOGIC_1 : LED_LOGIC_0;// B channel
			}
		}else{
			for(uint8_t j = 0; j < 8; j++){
				led_buffer[(i * 24) +       j]   = LED_LOGIC_0;// G channel
				led_buffer[(i * 24) + ( 8 + j)]  = LED_LOGIC_0;// R channel
				led_buffer[(i * 24) + (16 + j)]  = LED_LOGIC_0;// B channel
			}
		}
	}
}

void set_led_buffer_at_location(uint8_t pos, uint32_t color_rgb){
	for(uint8_t j = 0; j < 8; j++){
		led_buffer[(led_keymap[pos] * 24) +       j]   = (color_rgb & (1 << (15 - j))) ? LED_LOGIC_1 : LED_LOGIC_0;// G channel
		led_buffer[(led_keymap[pos] * 24) + ( 8 + j)]  = (color_rgb & (1 << (23 - j))) ? LED_LOGIC_1 : LED_LOGIC_0;// R channel
		led_buffer[(led_keymap[pos] * 24) + (16 + j)]  = (color_rgb & (1 << ( 7 - j))) ? LED_LOGIC_1 : LED_LOGIC_0;// B channel
	}
}

uint32_t calc_led_brightness(uint8_t brightness, uint32_t color_rgb){
	uint8_t r = (color_rgb >> 16) & 0xff;
	uint8_t g = (color_rgb >>  8) & 0xff;
	uint8_t b = (color_rgb      ) & 0xff;

	if(brightness > MAX_brightness)
		brightness = MAX_brightness;

	r = (r * brightness) / 255;
	g = (g * brightness) / 255;
	b = (b * brightness) / 255;
	return (r << 16 | g << 8 | b);
}
/* USER CODE END Application */

