/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2024 STMicroelectronics.
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

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
typedef struct Relay {
	GPIO_TypeDef *port;
	uint16_t pin;
} Relay;
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define UART_FRAME 22
#define QUANTITY_OF_RELAY 12
#define ON 1
#define OFF 0
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
TIM_HandleTypeDef htim2;

UART_HandleTypeDef huart2;

/* USER CODE BEGIN PV */
char rxByte[UART_FRAME];
char char_to_remove = '-';
char tmp_arr[4][20];
uint32_t start_time, current_time;
char result_after_remove[100];
Relay list_relay[QUANTITY_OF_RELAY] = { 0 };
int index_relay = 0;
bool flag_sequential_mode;
char sequential_mode[UART_FRAME] = "Sequential mode-------";
char stop_sequential_mode[UART_FRAME] = "Stop sequential mode--";
char word_sequential[] = "Sequential at";
char on[] = "Turn on";
char off[] = "Turn off";
char result[100];
//for debug
uint8_t StateDir = 0;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_TIM2_Init(void);
static void MX_USART2_UART_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
#if defined(__GNUC__)
int _write(int fd, char *ptr, int len) {
	HAL_UART_Transmit(&huart2, (uint8_t*) ptr, len, HAL_MAX_DELAY);
	return len;
}
#elif defined(__ICCARM__)#include "LowLevelIOInterface.h"

size_t __write(int handle,
  const unsigned char * buffer, size_t size) {
  HAL_UART_Transmit( &huart2, (uint8_t * ) buffer, size, UART_Delay);
  return size;
}
#elif defined(__CC_ARM)
int fputc(int ch, FILE * f) {
  HAL_UART_Transmit( &huart2, (uint8_t * ) &ch, 1, UART_Delay);
  return ch;
}
#endif
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
	if (htim->Instance == TIM2) {
		if (flag_sequential_mode) {
			current_time = HAL_GetTick();
			printf("Time for sequential: %ld ms\n",
					(current_time - start_time));
			HAL_GPIO_WritePin(list_relay[index_relay].port,
					list_relay[index_relay].pin, OFF);
			printf("Turn off Relay %d\n", index_relay + 1);
			index_relay++;
			if (index_relay >= 12) {
				index_relay = 0;
			}
			HAL_GPIO_WritePin(list_relay[index_relay].port,
					list_relay[index_relay].pin, ON);
			printf("Turn on Relay %d\n", index_relay + 1);
			start_time = HAL_GetTick();
		}
	}
}
void remove_multiple_character(char *str, char char_to_remove) {
	for (int i = 0; i < strlen(str); i++) {
		if (str[i] == char_to_remove) {
			str[i] = '\0';
		}
	}
}
void extract_data(char str[]) {
	int index = 0;
	char *tmp = strtok(str, " ");
	while (tmp != NULL) {
		strcpy(tmp_arr[index++], tmp);
		tmp = strtok(NULL, " ");
	}
	strcat(result_after_remove, tmp_arr[0]);
	strcat(result_after_remove, " ");
	strcat(result_after_remove, tmp_arr[1]);

}
void reset_value_uart(void) {
	tmp_arr[0][20] = '\0';
	index_relay = 0;
	flag_sequential_mode = false;
	result_after_remove[0] = '\0';
}
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
	if (huart->Instance == USART2) {
		reset_value_uart();
		HAL_UART_Receive_IT(&huart2, (uint8_t*) rxByte, UART_FRAME);
		if (!strncmp(rxByte, sequential_mode, UART_FRAME)) {
			flag_sequential_mode = true;
			HAL_GPIO_WritePin(list_relay[0].port, list_relay[0].pin, ON);
			__HAL_TIM_CLEAR_IT(&htim2, TIM_IT_UPDATE); // chong nhieu interrupt
			HAL_TIM_Base_Start_IT(&htim2);
			printf("Sequential mode & Turn on relay 1 \n");
			start_time = HAL_GetTick();
			return;
		} else if (!strncmp(rxByte, stop_sequential_mode, UART_FRAME)) {
			flag_sequential_mode = false;
			printf("Stop sequential mode\n");
			HAL_TIM_Base_Stop_IT(&htim2);
			return;
		}
		remove_multiple_character(rxByte, char_to_remove);
		extract_data(rxByte);
		index_relay = ((int) *tmp_arr[3] - '0') - 1;
		if (!strncmp(result_after_remove, word_sequential,
				strlen(word_sequential))) {
			HAL_GPIO_WritePin(list_relay[index_relay].port,
					list_relay[index_relay].pin, ON);
			printf("Turn on Relay %d\n", index_relay + 1);
			__HAL_TIM_CLEAR_IT(&htim2, TIM_IT_UPDATE); // chong nhieu interrupt
			HAL_TIM_Base_Start_IT(&htim2);
			flag_sequential_mode = true;
			start_time = HAL_GetTick();
		} else if (!strncmp(result_after_remove, on, strlen(on))) {
			HAL_GPIO_WritePin(list_relay[index_relay].port,
					list_relay[index_relay].pin, ON);
			printf("Turn on Relay %d\n", index_relay + 1);
		} else if (!strncmp(result_after_remove, off, strlen(on))) {
			HAL_GPIO_WritePin(list_relay[index_relay].port,
					list_relay[index_relay].pin, OFF);
			printf("Turn off Relay %d\n", index_relay + 1);
		}
	}
}
void init_relay_value(void) {
	list_relay[0] = (Relay ) { .port = GPIOC, .pin = RELAY1_Pin };
	list_relay[1] = (Relay ) { .port = GPIOC, .pin = RELAY2_Pin };
	list_relay[2] = (Relay ) { .port = GPIOB, .pin = RELAY3_Pin };
	list_relay[3] = (Relay ) { .port = GPIOB, .pin = RELAY4_Pin };
	list_relay[4] = (Relay ) { .port = GPIOB, .pin = RELAY5_Pin };
	list_relay[5] = (Relay ) { .port = GPIOE, .pin = RELAY6_Pin };
	list_relay[6] = (Relay ) { .port = GPIOE, .pin = RELAY7_Pin };
	list_relay[7] = (Relay ) { .port = GPIOE, .pin = RELAY8_Pin };
	list_relay[8] = (Relay ) { .port = GPIOE, .pin = RELAY9_Pin };
	list_relay[9] = (Relay ) { .port = GPIOE, .pin = RELAY10_Pin };
	list_relay[10] = (Relay ) { .port = GPIOE, .pin = RELAY11_Pin };
	list_relay[11] = (Relay ) { .port = GPIOE, .pin = RELAY12_Pin };
}
/* USER CODE END 0 */

/**
 * @brief  The application entry point.
 * @retval int
 */
int main(void) {

	/* USER CODE BEGIN 1 */

	/* USER CODE END 1 */

	/* MCU Configuration--------------------------------------------------------*/

	/* Reset of all peripherals, Initializes the Flash interface and the Systick. */
	HAL_Init();

	/* USER CODE BEGIN Init */

	/* USER CODE END Init */

	/* Configure the system clock */
	SystemClock_Config();

	/* USER CODE BEGIN SysInit */

	/* USER CODE END SysInit */

	/* Initialize all configured peripherals */
	MX_GPIO_Init();
	MX_TIM2_Init();
	MX_USART2_UART_Init();
	/* USER CODE BEGIN 2 */
	init_relay_value();
	__HAL_TIM_CLEAR_IT(&htim2, TIM_IT_UPDATE);
	HAL_UART_Receive_IT(&huart2, (uint8_t*) rxByte, UART_FRAME);
	/* USER CODE END 2 */

	/* Infinite loop */
	/* USER CODE BEGIN WHILE */
	while (1) {
		/* USER CODE END WHILE */

		/* USER CODE BEGIN 3 */
		StateDir = HAL_GPIO_ReadPin(list_relay[0].port, list_relay[0].pin);
	}
	/* USER CODE END 3 */
}

/**
 * @brief System Clock Configuration
 * @retval None
 */
void SystemClock_Config(void) {
	RCC_OscInitTypeDef RCC_OscInitStruct = { 0 };
	RCC_ClkInitTypeDef RCC_ClkInitStruct = { 0 };

	/** Configure the main internal regulator output voltage
	 */
	__HAL_RCC_PWR_CLK_ENABLE();
	__HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

	/** Initializes the RCC Oscillators according to the specified parameters
	 * in the RCC_OscInitTypeDef structure.
	 */
	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
	RCC_OscInitStruct.HSIState = RCC_HSI_ON;
	RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
	RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
	RCC_OscInitStruct.PLL.PLLM = 8;
	RCC_OscInitStruct.PLL.PLLN = 168;
	RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
	RCC_OscInitStruct.PLL.PLLQ = 4;
	if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
		Error_Handler();
	}

	/** Initializes the CPU, AHB and APB buses clocks
	 */
	RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK
			| RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
	RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

	if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK) {
		Error_Handler();
	}
}

/**
 * @brief TIM2 Initialization Function
 * @param None
 * @retval None
 */
static void MX_TIM2_Init(void) {

	/* USER CODE BEGIN TIM2_Init 0 */

	/* USER CODE END TIM2_Init 0 */

	TIM_ClockConfigTypeDef sClockSourceConfig = { 0 };
	TIM_MasterConfigTypeDef sMasterConfig = { 0 };

	/* USER CODE BEGIN TIM2_Init 1 */

	/* USER CODE END TIM2_Init 1 */
	htim2.Instance = TIM2;
	htim2.Init.Prescaler = 840 - 1;
	htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
	htim2.Init.Period = 1500000 - 1;
	htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
	if (HAL_TIM_Base_Init(&htim2) != HAL_OK) {
		Error_Handler();
	}
	sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
	if (HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig) != HAL_OK) {
		Error_Handler();
	}
	sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
	sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
	if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig)
			!= HAL_OK) {
		Error_Handler();
	}
	/* USER CODE BEGIN TIM2_Init 2 */

	/* USER CODE END TIM2_Init 2 */

}

/**
 * @brief USART2 Initialization Function
 * @param None
 * @retval None
 */
static void MX_USART2_UART_Init(void) {

	/* USER CODE BEGIN USART2_Init 0 */

	/* USER CODE END USART2_Init 0 */

	/* USER CODE BEGIN USART2_Init 1 */

	/* USER CODE END USART2_Init 1 */
	huart2.Instance = USART2;
	huart2.Init.BaudRate = 115200;
	huart2.Init.WordLength = UART_WORDLENGTH_8B;
	huart2.Init.StopBits = UART_STOPBITS_1;
	huart2.Init.Parity = UART_PARITY_NONE;
	huart2.Init.Mode = UART_MODE_TX_RX;
	huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	huart2.Init.OverSampling = UART_OVERSAMPLING_16;
	if (HAL_UART_Init(&huart2) != HAL_OK) {
		Error_Handler();
	}
	/* USER CODE BEGIN USART2_Init 2 */

	/* USER CODE END USART2_Init 2 */

}

/**
 * @brief GPIO Initialization Function
 * @param None
 * @retval None
 */
static void MX_GPIO_Init(void) {
	GPIO_InitTypeDef GPIO_InitStruct = { 0 };
	/* USER CODE BEGIN MX_GPIO_Init_1 */
	/* USER CODE END MX_GPIO_Init_1 */

	/* GPIO Ports Clock Enable */
	__HAL_RCC_GPIOH_CLK_ENABLE();
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOC_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();
	__HAL_RCC_GPIOE_CLK_ENABLE();

	/*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(GPIOC, RELAY1_Pin | RELAY2_Pin, GPIO_PIN_RESET);

	/*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(GPIOB, RELAY3_Pin | RELAY4_Pin | RELAY5_Pin,
			GPIO_PIN_RESET);

	/*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(GPIOE,
			RELAY6_Pin | RELAY7_Pin | RELAY8_Pin | RELAY9_Pin | RELAY10_Pin
					| RELAY11_Pin | RELAY12_Pin, GPIO_PIN_RESET);

	/*Configure GPIO pins : RELAY1_Pin RELAY2_Pin */
	GPIO_InitStruct.Pin = RELAY1_Pin | RELAY2_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

	/*Configure GPIO pins : RELAY3_Pin RELAY4_Pin RELAY5_Pin */
	GPIO_InitStruct.Pin = RELAY3_Pin | RELAY4_Pin | RELAY5_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

	/*Configure GPIO pins : RELAY6_Pin RELAY7_Pin RELAY8_Pin RELAY9_Pin
	 RELAY10_Pin RELAY11_Pin RELAY12_Pin */
	GPIO_InitStruct.Pin = RELAY6_Pin | RELAY7_Pin | RELAY8_Pin | RELAY9_Pin
			| RELAY10_Pin | RELAY11_Pin | RELAY12_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

	/* USER CODE BEGIN MX_GPIO_Init_2 */
	/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
 * @brief  This function is executed in case of error occurrence.
 * @retval None
 */
void Error_Handler(void) {
	/* USER CODE BEGIN Error_Handler_Debug */
	/* User can add his own implementation to report the HAL error return state */
	__disable_irq();
	while (1) {
	}
	/* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
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
