/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
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
#include "stm32wbxx_hal.h"

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
#define H_SENSOR_1_Pin GPIO_PIN_0
#define H_SENSOR_1_GPIO_Port GPIOC
#define H_SENSOR_2_Pin GPIO_PIN_1
#define H_SENSOR_2_GPIO_Port GPIOC
#define H_SENSOR_3_Pin GPIO_PIN_2
#define H_SENSOR_3_GPIO_Port GPIOC
#define H_SENSOR_4_Pin GPIO_PIN_3
#define H_SENSOR_4_GPIO_Port GPIOC
#define H_SENSOR_5_Pin GPIO_PIN_0
#define H_SENSOR_5_GPIO_Port GPIOA
#define H_SENSOR_6_Pin GPIO_PIN_1
#define H_SENSOR_6_GPIO_Port GPIOA
#define H_SENSOR_7_Pin GPIO_PIN_2
#define H_SENSOR_7_GPIO_Port GPIOA
#define H_SENSOR_8_Pin GPIO_PIN_3
#define H_SENSOR_8_GPIO_Port GPIOA
#define H_SENSOR_9_Pin GPIO_PIN_4
#define H_SENSOR_9_GPIO_Port GPIOA
#define H_SENSOR_10_Pin GPIO_PIN_5
#define H_SENSOR_10_GPIO_Port GPIOA
#define Valve1_Pin GPIO_PIN_6
#define Valve1_GPIO_Port GPIOA
#define Valve2_Pin GPIO_PIN_7
#define Valve2_GPIO_Port GPIOA
#define Valve3_Pin GPIO_PIN_8
#define Valve3_GPIO_Port GPIOA
#define B1_Pin GPIO_PIN_4
#define B1_GPIO_Port GPIOC

#define Valve7_Pin GPIO_PIN_12
#define Valve7_GPIO_Port GPIOB
#define Valve8_Pin GPIO_PIN_13
#define Valve8_GPIO_Port GPIOB
#define Valve9_Pin GPIO_PIN_14
#define Valve9_GPIO_Port GPIOB
#define Valve10_Pin GPIO_PIN_15
#define Valve10_GPIO_Port GPIOB
#define Valve4_Pin GPIO_PIN_11
#define Valve4_GPIO_Port GPIOA
#define Valve5_Pin GPIO_PIN_12
#define Valve5_GPIO_Port GPIOA
#define Valve6_Pin GPIO_PIN_15
#define Valve6_GPIO_Port GPIOA
#define DHT11_PIN_Pin GPIO_PIN_11
#define DHT11_PIN_GPIO_Port GPIOC

//Original from .ioc - IDE code generation for WB seems to brake up code

//#define DHT11_PIN_Pin GPIO_PIN_11
//#define DHT11_PIN_GPIO_Port GPIOC
//#define LD1_Pin GPIO_PIN_5
//#define LD1_GPIO_Port GPIOB
//#define LD2_Green_Led__Pin GPIO_PIN_0
//#define LD2_Green_Led__GPIO_Port GPIOB
//#define LD3_Pin GPIO_PIN_1
//#define LD3_GPIO_Port GPIOB

#define B2_Pin GPIO_PIN_0
#define B2_GPIO_Port GPIOD
#define B3_Pin GPIO_PIN_1
#define B3_GPIO_Port GPIOD

/* USER CODE BEGIN Private defines */
#define DHT11_Pin 		GPIO_PIN_11
#define DHT11_GPIO_Port GPIOC

#define BLUE_LED_Pin 	GPIO_PIN_5
#define BLUE_LED_Port 	GPIOB
#define GREEN_LED_Pin 	GPIO_PIN_0
#define GREEN_LED_Port 	GPIOB
#define RED_LED_Pin 	GPIO_PIN_1
#define RED_LED_Port 	GPIOB
/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
