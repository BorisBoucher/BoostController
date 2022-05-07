/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
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

extern ADC_HandleTypeDef hadc1;
extern DMA_HandleTypeDef hdma_adc1;

extern CAN_HandleTypeDef hcan;

extern RTC_HandleTypeDef hrtc;

extern TIM_HandleTypeDef htim1;
extern TIM_HandleTypeDef htim2;

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim);

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define MAP_Pin GPIO_PIN_0
#define MAP_GPIO_Port GPIOA
#define FUEL_PRES_Pin GPIO_PIN_1
#define FUEL_PRES_GPIO_Port GPIOA
#define OIL_PRES_Pin GPIO_PIN_2
#define OIL_PRES_GPIO_Port GPIOA
#define WATER_TEMP_Pin GPIO_PIN_3
#define WATER_TEMP_GPIO_Port GPIOA
#define OIL_TEMP_Pin GPIO_PIN_4
#define OIL_TEMP_GPIO_Port GPIOA
#define TPS_Pin GPIO_PIN_5
#define TPS_GPIO_Port GPIOA
#define AFR_Pin GPIO_PIN_6
#define AFR_GPIO_Port GPIOA
#define MAF_HZ_Pin GPIO_PIN_10
#define MAF_HZ_GPIO_Port GPIOB
#define WG_PWM_Pin GPIO_PIN_8
#define WG_PWM_GPIO_Port GPIOA
#define FUEL_P_Pin GPIO_PIN_10
#define FUEL_P_GPIO_Port GPIOA
#define RPM_HZ_Pin GPIO_PIN_15
#define RPM_HZ_GPIO_Port GPIOA
#define SPEED_HZ_Pin GPIO_PIN_3
#define SPEED_HZ_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
