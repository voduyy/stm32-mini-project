/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define RELAY1_Pin GPIO_PIN_4
#define RELAY1_GPIO_Port GPIOC
#define RELAY2_Pin GPIO_PIN_5
#define RELAY2_GPIO_Port GPIOC
#define RELAY3_Pin GPIO_PIN_0
#define RELAY3_GPIO_Port GPIOB
#define RELAY4_Pin GPIO_PIN_1
#define RELAY4_GPIO_Port GPIOB
#define RELAY5_Pin GPIO_PIN_2
#define RELAY5_GPIO_Port GPIOB
#define RELAY6_Pin GPIO_PIN_7
#define RELAY6_GPIO_Port GPIOE
#define RELAY7_Pin GPIO_PIN_8
#define RELAY7_GPIO_Port GPIOE
#define RELAY8_Pin GPIO_PIN_9
#define RELAY8_GPIO_Port GPIOE
#define RELAY9_Pin GPIO_PIN_10
#define RELAY9_GPIO_Port GPIOE
#define RELAY10_Pin GPIO_PIN_11
#define RELAY10_GPIO_Port GPIOE
#define RELAY11_Pin GPIO_PIN_12
#define RELAY11_GPIO_Port GPIOE
#define RELAY12_Pin GPIO_PIN_13
#define RELAY12_GPIO_Port GPIOE

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
