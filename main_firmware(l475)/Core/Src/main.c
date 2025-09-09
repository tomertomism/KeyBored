/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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
#include "main.h"
#include "dma.h"
#include "i2c.h"
#include "rtc.h"
#include "spi.h"
#include "tim.h"
#include "usb_device.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "usbd_custom_hid_if.h"
#include "custom_keymap.h"

#include <string.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
uint8_t countdown[5];

uint8_t key_report[9];
uint8_t consumer_report[2];

uint8_t is_fn_key_pressed;

uint8_t is_consumer_key_pressed = 0;
uint8_t caps_lock_status = 0;
uint8_t win_key_enable = 0;

uint8_t esp_hid_report_desc[USBD_CUSTOM_HID_REPORT_DESC_SIZE + 1];
uint8_t esp_key_report[8] = {1, 0, 0, 0, 0, 0, 0, 0};
uint8_t spi3_rx_buffer[128];
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
void scan_code(uint8_t layer);

void layer0_handler(uint8_t *report1, uint8_t *report2);

void layer1_handler(uint8_t *report1, uint8_t *report2);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */
  __enable_irq();

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */
  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_TIM2_Init();
  MX_TIM1_Init();
  MX_SPI1_Init();
  MX_SPI3_Init();
  MX_I2C1_Init();
  MX_TIM15_Init();
  MX_RTC_Init();
  MX_USB_DEVICE_Init();
  MX_TIM5_Init();
  MX_TIM16_Init();
  /* USER CODE BEGIN 2 */
  fill_led_buffer(0);
  HAL_TIM_PWM_Start_DMA(&htim2, TIM_CHANNEL_4, (uint32_t *) led_buffer, LED_BUFFER_SIZE);
  esp_hid_report_desc[0] = USBD_CUSTOM_HID_REPORT_DESC_SIZE;
  memcpy(&esp_hid_report_desc[1], CUSTOM_HID_ReportDesc_FS, USBD_CUSTOM_HID_REPORT_DESC_SIZE);
  HAL_SPI_Receive_IT(&hspi3, spi3_rx_buffer, 12);
  ESPEN_GPIO_Port->BSRR = ESPEN_Pin;
  HAL_TIM_Base_Start_IT(&htim15);
  HAL_TIM_Base_Start(&htim16);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
	  scan_code(is_fn_key_pressed);
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure LSE Drive Capability
  */
  HAL_PWR_EnableBkUpAccess();
  __HAL_RCC_LSEDRIVE_CONFIG(RCC_LSEDRIVE_LOW);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE|RCC_OSCILLATORTYPE_LSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.LSEState = RCC_LSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 1;
  RCC_OscInitStruct.PLL.PLLN = 20;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV7;
  RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV6;
  RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
  {
    Error_Handler();
  }

  /** Enables the Clock Security System
  */
  HAL_RCC_EnableCSS();
}

/* USER CODE BEGIN 4 */
void scan_code(uint8_t layer){
	uint8_t report1[9] = {1, 0, 0, 0, 0, 0, 0, 0, 0};
	uint8_t report2[2] = {2, 0};
	switch(layer){
	case 0:
		layer0_handler(report1, report2);
		break;
	case 1:
		layer1_handler(report1, report2);
		break;
	}
	memcpy(key_report     , report1, 9);
	memcpy(consumer_report, report2, 2);
	memcpy(&esp_key_report[1], &report1[3], 6);
	memcpy(&esp_key_report[7], &report2[1], 1);
}

void layer0_handler(uint8_t *report1, uint8_t *report2){
	uint8_t count = 3;
	uint8_t size = 9;

	row0_set;

	if(count < size && col0_offset)
		report1[count++] = keymap_1[0];
	if(                col1_offset){
		if(!is_consumer_key_pressed){
			if(!countdown[0]){
				report2[1] = keymap_1[1];
				is_consumer_key_pressed = 1;
			}
			countdown[0] = 50;
		}
	}
	if(count < size && col2_offset)
		report1[count++] = keymap_1[2];
	if(count < size && col3_offset)
		report1[count++] = keymap_1[3];
	if(count < size && col4_offset)
		report1[count++] = keymap_1[4];
	if(count < size && col5_offset)
		report1[count++] = keymap_1[5];
	if(count < size && col6_offset)
		report1[count++] = keymap_1[6];
	if(count < size && col7_offset)
		report1[count++] = keymap_1[7];
	if(count < size && col8_offset)
		report1[count++] = keymap_1[8];
	if(count < size && col9_offset)
		report1[count++] = keymap_1[9];
	if(count < size && col10_offset)
		report1[count++] = keymap_1[10];
	if(count < size && col11_offset)
		report1[count++] = keymap_1[11];
	if(count < size && col12_offset)
		report1[count++] = keymap_1[12];
	if(count < size && col13_offset)
		report1[count++] = keymap_1[13];
	if(count < size && col14_offset)
		report1[count++] = keymap_1[14];

	row0_reset;
	TIM16->CNT = 0;
	while(TIM16->CNT < row_scan_interval);
	row1_set;

	if(count < size && col0_offset)
		report1[count++] = keymap_1[15];
	if(count < size && col1_offset)
		report1[count++] = keymap_1[16];
	if(count < size && col2_offset)
		report1[count++] = keymap_1[17];
	if(count < size && col3_offset)
		report1[count++] = keymap_1[18];
	if(count < size && col4_offset)
		report1[count++] = keymap_1[19];
	if(count < size && col5_offset)
		report1[count++] = keymap_1[20];
	if(count < size && col6_offset)
		report1[count++] = keymap_1[21];
	if(count < size && col7_offset)
		report1[count++] = keymap_1[22];
	if(count < size && col8_offset)
		report1[count++] = keymap_1[23];
	if(count < size && col9_offset)
		report1[count++] = keymap_1[24];
	if(count < size && col10_offset)
		report1[count++] = keymap_1[25];
	if(count < size && col11_offset)
		report1[count++] = keymap_1[26];
	if(count < size && col12_offset)
		report1[count++] = keymap_1[27];
	if(count < size && col13_offset)
		report1[count++] = keymap_1[28];
	if(count < size && col14_offset)
		report1[count++] = keymap_1[29];

	row1_reset;
	TIM16->CNT = 0;
	while(TIM16->CNT < row_scan_interval);
	row2_set;

	if(count < size && col0_offset)
		report1[count++] = keymap_1[30];
	if(count < size && col1_offset)
		report1[count++] = keymap_1[31];
	if(count < size && col2_offset)
		report1[count++] = keymap_1[32];
	if(count < size && col3_offset)
		report1[count++] = keymap_1[33];
	if(count < size && col4_offset)
		report1[count++] = keymap_1[34];
	if(count < size && col5_offset)
		report1[count++] = keymap_1[35];
	if(count < size && col6_offset)
		report1[count++] = keymap_1[36];
	if(count < size && col7_offset)
		report1[count++] = keymap_1[37];
	if(count < size && col8_offset)
		report1[count++] = keymap_1[38];
	if(count < size && col9_offset)
		report1[count++] = keymap_1[39];
	if(count < size && col10_offset)
		report1[count++] = keymap_1[40];
	if(count < size && col11_offset)
		report1[count++] = keymap_1[41];
	if(count < size && col12_offset)
		report1[count++] = keymap_1[42];
	if(count < size && col13_offset)
		report1[count++] = keymap_1[43];
	if(count < size && col14_offset)
		report1[count++] = keymap_1[44];

	row2_reset;
	TIM16->CNT = 0;
	while(TIM16->CNT < row_scan_interval);
	row3_set;

	if(count < size && col0_offset)
		report1[count++] = keymap_1[45];
	if(count < size && col1_offset)
		report1[count++] = keymap_1[46];
	if(count < size && col2_offset)
		report1[count++] = keymap_1[47];
	if(count < size && col3_offset)
		report1[count++] = keymap_1[48];
	if(count < size && col4_offset)
		report1[count++] = keymap_1[49];
	if(count < size && col5_offset)
		report1[count++] = keymap_1[50];
	if(count < size && col6_offset)
		report1[count++] = keymap_1[51];
	if(count < size && col7_offset)
		report1[count++] = keymap_1[52];
	if(count < size && col8_offset)
		report1[count++] = keymap_1[53];
	if(count < size && col9_offset)
		report1[count++] = keymap_1[54];
	if(count < size && col10_offset)
		report1[count++] = keymap_1[55];
	if(count < size && col11_offset)
		report1[count++] = keymap_1[56];
	if(count < size && col12_offset)
		report1[count++] = keymap_1[57];
//	if(count < size && col13_offset);
	if(count < size && col14_offset)
		report1[count++] = keymap_1[58];

	row3_reset;
	TIM16->CNT = 0;
	while(TIM16->CNT < row_scan_interval);
	row4_set;

	if(                col0_offset)
		report1[     1] |= keymap_1[59];
	if(count < size && col1_offset)
		report1[count++] = keymap_1[60];
	if(count < size && col2_offset)
		report1[count++] = keymap_1[61];
	if(count < size && col3_offset)
		report1[count++] = keymap_1[62];
	if(count < size && col4_offset)
		report1[count++] = keymap_1[63];
	if(count < size && col5_offset)
		report1[count++] = keymap_1[64];
	if(count < size && col6_offset)
		report1[count++] = keymap_1[65];
	if(count < size && col7_offset)
		report1[count++] = keymap_1[66];
	if(count < size && col8_offset)
		report1[count++] = keymap_1[67];
	if(count < size && col9_offset)
		report1[count++] = keymap_1[68];
	if(count < size && col10_offset)
		report1[count++] = keymap_1[69];
	if(count < size && col11_offset)
		report1[count++] = keymap_1[70];
	if(count < size && col12_offset)
		report1[count++] = keymap_1[71];
//	if(count < size && col13_offset);
	if(count < size && col14_offset)
		report1[count++] = keymap_1[72];

	row4_reset;
	TIM16->CNT = 0;
	while(TIM16->CNT < row_scan_interval);
	row5_set;

	if(                col0_offset)
		report1[1] |= keymap_1[73];
	if(                col1_offset)
		report1[1] |= keymap_1[74];
	if(                col2_offset)
		report1[1] |= keymap_1[75];
//	if(count < size && col3_offset);
//	if(count < size && col4_offset);
	if(count < size && col5_offset)
		report1[count++] = keymap_1[76];
//	if(count < size && col6_offset);
//	if(count < size && col7_offset);
//	if(count < size && col8_offset);
	if(count < size && col9_offset)
		report1[count++] = keymap_1[77];
	if(               col10_offset){
		is_fn_key_pressed = 1;
	}else{
		is_fn_key_pressed = 0;
	}
	if(count < size && col11_offset)
		report1[count++] = keymap_1[79];
	if(count < size && col12_offset)
		report1[count++] = keymap_1[80];
	if(count < size && col13_offset)
		report1[count++] = keymap_1[81];
	if(count < size && col14_offset)
		report1[count++] = keymap_1[82];

	row5_reset;
	TIM16->CNT = 0;
	while(TIM16->CNT < row_scan_interval);
}

void layer1_handler(uint8_t *report1, uint8_t *report2){
	uint8_t count = 3;
	uint8_t size = 9;

	row0_set;

	if(count < size && col0_offset);
	if(count < size && col1_offset);
	if(count < size && col2_offset);
	if(count < size && col3_offset);
	if(count < size && col4_offset);
	if(count < size && col5_offset);
	if(count < size && col6_offset);
	if(count < size && col7_offset);
	if(count < size && col8_offset);
	if(count < size && col9_offset);
	if(count < size && col10_offset);
	if(count < size && col11_offset);
	if(count < size && col12_offset);
	if(count < size && col13_offset);
	if(count < size && col14_offset);
	if(count < size && col15_offset);

	row0_reset;
	row1_set;

	if(count < size && col0_offset);
	if(count < size && col1_offset);
	if(count < size && col2_offset);
	if(count < size && col3_offset);
	if(count < size && col4_offset);
	if(count < size && col5_offset);
	if(count < size && col6_offset);
	if(count < size && col7_offset);
	if(count < size && col8_offset);
	if(count < size && col9_offset);
	if(count < size && col10_offset);
	if(count < size && col11_offset);
	if(count < size && col12_offset);
	if(count < size && col13_offset);
	if(count < size && col14_offset);
	if(count < size && col15_offset);

	row1_reset;
	row2_set;

	if(count < size && col0_offset);
	if(count < size && col1_offset);
	if(count < size && col2_offset);
	if(count < size && col3_offset);
	if(count < size && col4_offset);
	if(count < size && col5_offset);
	if(count < size && col6_offset);
	if(count < size && col7_offset);
	if(count < size && col8_offset);
	if(count < size && col9_offset);
	if(count < size && col10_offset);
	if(count < size && col11_offset);
	if(count < size && col12_offset);
	if(count < size && col13_offset);
	if(count < size && col14_offset);
	if(count < size && col15_offset);

	row2_reset;
	row3_set;

	if(count < size && col0_offset);
	if(count < size && col1_offset);
	if(count < size && col2_offset);
	if(count < size && col3_offset);
	if(count < size && col4_offset);
	if(count < size && col5_offset);
	if(count < size && col6_offset);
	if(count < size && col7_offset);
	if(count < size && col8_offset);
	if(count < size && col9_offset);
	if(count < size && col10_offset);
	if(count < size && col11_offset);
	if(count < size && col12_offset);
	if(count < size && col13_offset);
	if(count < size && col14_offset);
	if(count < size && col15_offset);

	row3_reset;
	row4_set;

	if(count < size && col0_offset);
	if(count < size && col1_offset);
	if(count < size && col2_offset);
	if(count < size && col3_offset);
	if(count < size && col4_offset);
	if(count < size && col5_offset);
	if(count < size && col6_offset);
	if(count < size && col7_offset);
	if(count < size && col8_offset);
	if(count < size && col9_offset);
	if(count < size && col10_offset);
	if(count < size && col11_offset);
	if(count < size && col12_offset);
	if(count < size && col13_offset);
	if(count < size && col14_offset);
	if(count < size && col15_offset);

	row4_reset;
	row5_set;

	if(count < size && col0_offset);
	if(count < size && col1_offset);
	if(count < size && col2_offset);
	if(count < size && col3_offset);
	if(count < size && col4_offset);
	if(count < size && col5_offset);
	if(count < size && col6_offset);
	if(count < size && col7_offset);
	if(count < size && col8_offset);
	if(count < size && col9_offset);
	if(count < size && col10_offset);
	if(count < size && col11_offset);
	if(count < size && col12_offset);
	if(count < size && col13_offset);
	if(count < size && col14_offset);
	if(count < size && col15_offset);
	row5_reset;
}

void fill_led_buffer(uint32_t color_rgb){
	for(uint16_t i = 0; i < NUM_LED; i++){
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

void HAL_SPI_RxCpltCallback(SPI_HandleTypeDef * hspi){
	if(hspi->Instance == SPI3){
//		memset(spi3_tx_buffer, 0, 16);
		switch(spi3_rx_buffer[0]){
		case 0x1:
			HAL_SPI_Transmit_IT(hspi, esp_hid_report_desc, sizeof(esp_hid_report_desc));
			break;
		case 0x11:
			HAL_SPI_Transmit_IT(hspi, esp_key_report, sizeof(esp_key_report));
			break;
		default:
			HAL_SPI_Receive_IT(hspi, spi3_rx_buffer, 12);
			break;
		}
	}
}

void HAL_SPI_TxCpltCallback(SPI_HandleTypeDef * hspi){
	if(hspi->Instance == SPI3){
		HAL_SPI_Receive_IT(hspi, spi3_rx_buffer, 12);
	}
}
void HAL_SPI_ErrorCallback(SPI_HandleTypeDef *hspi){
	NVIC_SystemReset();
}
/* USER CODE END 4 */

/**
  * @brief  Period elapsed callback in non blocking mode
  * @note   This function is called  when TIM17 interrupt took place, inside
  * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
  * a global variable "uwTick" used as application time base.
  * @param  htim : TIM handle
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  /* USER CODE BEGIN Callback 0 */
  if(htim->Instance == TIM15){
	  if(is_consumer_key_pressed){
		  USBD_CUSTOM_HID_SendReport(&hUsbDeviceFS, consumer_report, 2);
	  }else{
		  USBD_CUSTOM_HID_SendReport(&hUsbDeviceFS, key_report, 9);
	  }
  }

  /* USER CODE END Callback 0 */
  if (htim->Instance == TIM17)
  {
    HAL_IncTick();
  }
  /* USER CODE BEGIN Callback 1 */

  /* USER CODE END Callback 1 */
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1);
  /* USER CODE END Error_Handler_Debug */
}
#ifdef USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
