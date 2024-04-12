/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"

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
#define POWER_Pin GPIO_PIN_15
#define POWER_GPIO_Port GPIOC
#define POWER_EXTI_IRQn EXTI15_10_IRQn
#define BLOCK_R_Pin GPIO_PIN_3
#define BLOCK_R_GPIO_Port GPIOA
#define BLOCK_R_EXTI_IRQn EXTI3_IRQn
#define BLOCK_B_Pin GPIO_PIN_4
#define BLOCK_B_GPIO_Port GPIOA
#define BLOCK_B_EXTI_IRQn EXTI4_IRQn
#define BLOCK_G_Pin GPIO_PIN_5
#define BLOCK_G_GPIO_Port GPIOA
#define BLOCK_G_EXTI_IRQn EXTI9_5_IRQn
#define OFF_3_Pin GPIO_PIN_10
#define OFF_3_GPIO_Port GPIOB
#define ON_3_Pin GPIO_PIN_11
#define ON_3_GPIO_Port GPIOB
#define OFF_1_Pin GPIO_PIN_15
#define OFF_1_GPIO_Port GPIOB
#define ON_1_Pin GPIO_PIN_8
#define ON_1_GPIO_Port GPIOA
#define ON_Pin GPIO_PIN_15
#define ON_GPIO_Port GPIOA
#define OFF_Pin GPIO_PIN_3
#define OFF_GPIO_Port GPIOB
#define ON_2_Pin GPIO_PIN_4
#define ON_2_GPIO_Port GPIOB
#define OFF_2_Pin GPIO_PIN_5
#define OFF_2_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
