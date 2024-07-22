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
#include "stm32g4xx_hal.h"

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
#define ON_BOARD_LED_Pin GPIO_PIN_13
#define ON_BOARD_LED_GPIO_Port GPIOC
#define MCU_ADC0_Pin GPIO_PIN_0
#define MCU_ADC0_GPIO_Port GPIOA
#define MCU_ADC1_Pin GPIO_PIN_1
#define MCU_ADC1_GPIO_Port GPIOA
#define MCU_ADC2_Pin GPIO_PIN_2
#define MCU_ADC2_GPIO_Port GPIOA
#define MCU_ADC3_Pin GPIO_PIN_3
#define MCU_ADC3_GPIO_Port GPIOA
#define MCU_DAC1_Pin GPIO_PIN_4
#define MCU_DAC1_GPIO_Port GPIOA
#define MCU_DAC2_Pin GPIO_PIN_6
#define MCU_DAC2_GPIO_Port GPIOA
#define MCU_ADC11_Pin GPIO_PIN_12
#define MCU_ADC11_GPIO_Port GPIOB
#define TEST_PIN_Pin GPIO_PIN_13
#define TEST_PIN_GPIO_Port GPIOB
#define LED3_Pin GPIO_PIN_15
#define LED3_GPIO_Port GPIOB
#define MT_EN_Pin GPIO_PIN_8
#define MT_EN_GPIO_Port GPIOA
#define MT_PH_Pin GPIO_PIN_9
#define MT_PH_GPIO_Port GPIOA
#define LED1_Pin GPIO_PIN_11
#define LED1_GPIO_Port GPIOA
#define LED2_Pin GPIO_PIN_12
#define LED2_GPIO_Port GPIOA
#define BTN3_Pin GPIO_PIN_3
#define BTN3_GPIO_Port GPIOB
#define BTN1_Pin GPIO_PIN_4
#define BTN1_GPIO_Port GPIOB
#define BTN2_Pin GPIO_PIN_5
#define BTN2_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */