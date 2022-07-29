/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2022 STMicroelectronics.
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

void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim);

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define GPIO1_1_Pin GPIO_PIN_2
#define GPIO1_1_GPIO_Port GPIOE
#define GPIO1_2_Pin GPIO_PIN_3
#define GPIO1_2_GPIO_Port GPIOE
#define GPIO2_1_Pin GPIO_PIN_4
#define GPIO2_1_GPIO_Port GPIOE
#define GPIO2_2_Pin GPIO_PIN_5
#define GPIO2_2_GPIO_Port GPIOE
#define BLDC_1_Pin GPIO_PIN_1
#define BLDC_1_GPIO_Port GPIOC
#define BLDC_2_Pin GPIO_PIN_2
#define BLDC_2_GPIO_Port GPIOC
#define IG32Motor_PWM_Pin GPIO_PIN_2
#define IG32Motor_PWM_GPIO_Port GPIOA
#define Encoder1_Pin GPIO_PIN_5
#define Encoder1_GPIO_Port GPIOA
#define Motor3_PWM1_Pin GPIO_PIN_6
#define Motor3_PWM1_GPIO_Port GPIOA
#define Motor3_PWM2_Pin GPIO_PIN_7
#define Motor3_PWM2_GPIO_Port GPIOA
#define Motor3_DIR1_Pin GPIO_PIN_4
#define Motor3_DIR1_GPIO_Port GPIOC
#define motor3_DIR2_Pin GPIO_PIN_5
#define motor3_DIR2_GPIO_Port GPIOC
#define Encoder2_Pin GPIO_PIN_9
#define Encoder2_GPIO_Port GPIOE
#define GPIO3_1_Pin GPIO_PIN_11
#define GPIO3_1_GPIO_Port GPIOE
#define GPIO3_2_Pin GPIO_PIN_12
#define GPIO3_2_GPIO_Port GPIOE
#define HorizontalPickerMotor_DIR_Pin GPIO_PIN_9
#define HorizontalPickerMotor_DIR_GPIO_Port GPIOD
#define LiftingMotor_DIR_Pin GPIO_PIN_11
#define LiftingMotor_DIR_GPIO_Port GPIOD
#define LOCO_DIR1_Pin GPIO_PIN_12
#define LOCO_DIR1_GPIO_Port GPIOD
#define LOCO_DIR2_Pin GPIO_PIN_13
#define LOCO_DIR2_GPIO_Port GPIOD
#define HorizontalPickerMotor_PWM_Pin GPIO_PIN_14
#define HorizontalPickerMotor_PWM_GPIO_Port GPIOD
#define LiftingMotor_PWM_Pin GPIO_PIN_15
#define LiftingMotor_PWM_GPIO_Port GPIOD
#define SpindleMotor_DIR_Pin GPIO_PIN_6
#define SpindleMotor_DIR_GPIO_Port GPIOC
#define IG32Motor_DIR_Pin GPIO_PIN_7
#define IG32Motor_DIR_GPIO_Port GPIOC
#define LOCO_PWM1_Pin GPIO_PIN_6
#define LOCO_PWM1_GPIO_Port GPIOB
#define LOCO_PWM2_Pin GPIO_PIN_7
#define LOCO_PWM2_GPIO_Port GPIOB
#define SpindleMotor_PWM_Pin GPIO_PIN_8
#define SpindleMotor_PWM_GPIO_Port GPIOB
#define STEPPER_DIR_Pin GPIO_PIN_9
#define STEPPER_DIR_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
