/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
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
#include "SimpleKalmanFilter.h"
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

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
ADC_HandleTypeDef hadc1;
ADC_HandleTypeDef hadc2;

/* USER CODE BEGIN PV */
uint32_t n_green, n_blue, n_red, now, start, done_r = 0, done_g = 0, done_b = 0;
uint8_t block_r = 1, block_b = 0, block_g = 0, ret = 0, mode = 0, flag_r = 0,
		flag_g = 0, flag_b = 0;
uint8_t count, mode_power = 0;
uint32_t Nadc_value[2];
SimpleKalmanFilter filter(2, 2, 0.01);
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_ADC1_Init(void);
static void MX_ADC2_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {

	if (GPIO_Pin == BLOCK_B_Pin) {
		if(mode==3)
			{block_b = 1;
		}
	} else if (GPIO_Pin == BLOCK_G_Pin) {
		if(mode==1)
			{block_g = 1;

			}
	}
	if (GPIO_Pin == POWER_Pin) {
		HAL_Delay(10);
		mode_power = 1 - mode_power;
		if (mode_power == 1) {
			HAL_GPIO_WritePin(GPIOB, OFF_Pin, GPIO_PIN_SET);
		} else {
			HAL_GPIO_WritePin(GPIOB, OFF_Pin, GPIO_PIN_RESET);
		}
		HAL_Delay(100);
		__HAL_GPIO_EXTI_CLEAR_IT(POWER_Pin);
		EXTI->PR = 1;
	}
}

void Select_ADC_Channel_0(void) {
	ADC_ChannelConfTypeDef sConfig = { 0 };
	sConfig.Channel = ADC_CHANNEL_0;
	sConfig.Rank = ADC_REGULAR_RANK_1;
	sConfig.SamplingTime = ADC_SAMPLETIME_239CYCLES_5;
	if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK) {
		Error_Handler();
	}
}
void Select_ADC_Channel_1(void) {
	ADC_ChannelConfTypeDef sConfig = { 0 };
	sConfig.Channel = ADC_CHANNEL_1;
	sConfig.Rank = ADC_REGULAR_RANK_1;
	sConfig.SamplingTime = ADC_SAMPLETIME_239CYCLES_5;
	if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK) {
		Error_Handler();
	}
}
void Get_Value_ADC1_CH0() {
	HAL_ADC_PollForConversion(&hadc1, 1000);
	n_green = HAL_ADC_GetValue(&hadc1);
	n_green = filter.updateEstimate(n_green);
}
void Get_Value_ADC1_CH1() {
	HAL_ADC_PollForConversion(&hadc1, 1000);
	n_blue = HAL_ADC_GetValue(&hadc1);
	n_blue = filter.updateEstimate(n_blue);

}
void Get_Value_ADC2() {
	HAL_ADC_PollForConversion(&hadc2, 1000);
	n_red = HAL_ADC_GetValue(&hadc2);
	n_red = filter.updateEstimate(n_red);

}
int Check_Mode() {
	switch (mode) {
	case 1:
		if (block_g == 1) {

			HAL_GPIO_WritePin(GPIOB, OFF_3_Pin, GPIO_PIN_SET);
			start = HAL_GetTick();
			while (1) {
				now = HAL_GetTick();
				if ((now - start) >= 1750) {
					break;
				}
			}
			HAL_GPIO_WritePin(GPIOB, OFF_3_Pin, GPIO_PIN_RESET);
			count = 0;
			block_g=0;
			done_g= 1;
		}
		break;
	case 2:
		if (block_r == 1) {

			HAL_GPIO_WritePin(GPIOB, OFF_1_Pin, GPIO_PIN_SET);
			start = HAL_GetTick();
			while (1) {
				now = HAL_GetTick();
				if ((now - start) >= 1750) {
					break;
				}
			}
			HAL_GPIO_WritePin(GPIOB, OFF_1_Pin, GPIO_PIN_RESET);
			block_r = 1;
			count = 0;
			done_r = 1;
		}
		break;
	case 3:

		if (block_b == 1) {
			HAL_GPIO_WritePin(GPIOB, OFF_2_Pin, GPIO_PIN_SET);
			start = HAL_GetTick();
			while (1) {
				now = HAL_GetTick();
				if ((now - start) >= 1750) {
					break;
				}
			}
			HAL_GPIO_WritePin(GPIOB, OFF_2_Pin, GPIO_PIN_RESET);
			block_b = 0;
			count = 0;
			done_b = 1;
		}
		break;
	}
	if (done_r == 1 || done_g == 1 || done_b == 1) {
		flag_r = 0;
		flag_g = 0;
		flag_b = 0;
		mode=0;
		return 1;
	}
	return 0;
}
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

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_ADC1_Init();
  MX_ADC2_Init();
  /* USER CODE BEGIN 2 */
	HAL_ADC_Start(&hadc1);
	HAL_ADC_Start(&hadc2);

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1){
		ADC_Channel_0();
		Get_Value_ADC1_CH0();
		if (n_green < 1025 && mode_power == 1) {
			mode = 1; // red
			//HAL_GPIO_WritePin(GPIOB,LED_RED_1_PIN,GPIO_PIN_SET);
			flag_g = 1;


		}
		if (flag_g == 1) {
			count++;
		}
		ADC_Channel_1();
		Get_Value_ADC1_CH1();
		if (n_blue < 1025 && mode_power == 1) {
			mode = 3;
			//HAL_GPIO_WritePin(GPIOB,LED_GREEN_1_PIN,GPIO_PIN_SET);
			flag_b = 1;
		}
		if (flag_b == 1) {
			count++;
		}

		Get_Value_ADC2();
		if (n_red < 1025 && mode_power == 1) {
			mode = 2;
			//HAL_GPIO_WritePin(GPIOB,LED_BLUE_1_PIN,GPIO_PIN_SET);
			flag_r = 1;
		}
		if (flag_r == 1) {
			count++;
		}

		while (count == 1) {
			ret = Check_Mode();
			if (ret == 1) {
				done_r = 0;
				done_g = 0;
				done_b = 0;
				block_b=0;
				block_g=0;
				ret=0;
				break;
			}
		}
	}
	if (count != 1) {
		count = 0;
		block_b=0;
		block_g=0;
		flag_r = 0;
		flag_g = 0;
		flag_b = 0;
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
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_ADC;
  PeriphClkInit.AdcClockSelection = RCC_ADCPCLK2_DIV2;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief ADC1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_ADC1_Init(void)
{

  /* USER CODE BEGIN ADC1_Init 0 */

  /* USER CODE END ADC1_Init 0 */

  ADC_ChannelConfTypeDef sConfig = {0};

  /* USER CODE BEGIN ADC1_Init 1 */

  /* USER CODE END ADC1_Init 1 */

  /** Common config
  */
  hadc1.Instance = ADC1;
  hadc1.Init.ScanConvMode = ADC_SCAN_DISABLE;
  hadc1.Init.ContinuousConvMode = ENABLE;
  hadc1.Init.DiscontinuousConvMode = DISABLE;
  hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc1.Init.NbrOfConversion = 1;
  if (HAL_ADC_Init(&hadc1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Regular Channel
  */
  sConfig.Channel = ADC_CHANNEL_0;
  sConfig.Rank = ADC_REGULAR_RANK_1;
  sConfig.SamplingTime = ADC_SAMPLETIME_239CYCLES_5;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN ADC1_Init 2 */

  /* USER CODE END ADC1_Init 2 */

}

/**
  * @brief ADC2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_ADC2_Init(void)
{

  /* USER CODE BEGIN ADC2_Init 0 */

  /* USER CODE END ADC2_Init 0 */

  ADC_ChannelConfTypeDef sConfig = {0};

  /* USER CODE BEGIN ADC2_Init 1 */

  /* USER CODE END ADC2_Init 1 */

  /** Common config
  */
  hadc2.Instance = ADC2;
  hadc2.Init.ScanConvMode = ADC_SCAN_DISABLE;
  hadc2.Init.ContinuousConvMode = ENABLE;
  hadc2.Init.DiscontinuousConvMode = DISABLE;
  hadc2.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc2.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc2.Init.NbrOfConversion = 1;
  if (HAL_ADC_Init(&hadc2) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Regular Channel
  */
  sConfig.Channel = ADC_CHANNEL_2;
  sConfig.Rank = ADC_REGULAR_RANK_1;
  sConfig.SamplingTime = ADC_SAMPLETIME_1CYCLE_5;
  if (HAL_ADC_ConfigChannel(&hadc2, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN ADC2_Init 2 */

  /* USER CODE END ADC2_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
/* USER CODE BEGIN MX_GPIO_Init_1 */
/* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, OFF_3_Pin|ON_3_Pin|OFF_1_Pin|OFF_Pin
                          |ON_2_Pin|OFF_2_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, ON_1_Pin|ON_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : POWER_Pin */
  GPIO_InitStruct.Pin = POWER_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(POWER_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : BLOCK_R_Pin BLOCK_B_Pin BLOCK_G_Pin */
  GPIO_InitStruct.Pin = BLOCK_R_Pin|BLOCK_B_Pin|BLOCK_G_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : OFF_3_Pin ON_3_Pin OFF_1_Pin OFF_Pin
                           ON_2_Pin OFF_2_Pin */
  GPIO_InitStruct.Pin = OFF_3_Pin|ON_3_Pin|OFF_1_Pin|OFF_Pin
                          |ON_2_Pin|OFF_2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pins : ON_1_Pin ON_Pin */
  GPIO_InitStruct.Pin = ON_1_Pin|ON_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI3_IRQn, 1, 0);
  HAL_NVIC_EnableIRQ(EXTI3_IRQn);

  HAL_NVIC_SetPriority(EXTI4_IRQn, 1, 0);
  HAL_NVIC_EnableIRQ(EXTI4_IRQn);

  HAL_NVIC_SetPriority(EXTI9_5_IRQn, 1, 0);
  HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);

  HAL_NVIC_SetPriority(EXTI15_10_IRQn, 1, 0);
  HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
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
