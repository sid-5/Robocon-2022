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
#define Limit_switch_input1_Pin GPIO_PIN_2
#define Limit_switch_input1_GPIO_Port GPIOE
#define Laser1_en_Pin GPIO_PIN_3
#define Laser1_en_GPIO_Port GPIOE
#define Liimit_switch_input2_Pin GPIO_PIN_4
#define Liimit_switch_input2_GPIO_Port GPIOE
#define GPIO2_2_Pin GPIO_PIN_5
#define GPIO2_2_GPIO_Port GPIOE
#define BLDC_1_Pin GPIO_PIN_1
#define BLDC_1_GPIO_Port GPIOC
#define BLDC_2_Pin GPIO_PIN_2
#define BLDC_2_GPIO_Port GPIOC
#define Servo_PWM_Pin GPIO_PIN_2
#define Servo_PWM_GPIO_Port GPIOA
#define Encoder1_Pin GPIO_PIN_5
#define Encoder1_GPIO_Port GPIOA
#define Motor3_PWM1_Pin GPIO_PIN_6
#define Motor3_PWM1_GPIO_Port GPIOA
#define LinearActuator_en_Pin GPIO_PIN_7
#define LinearActuator_en_GPIO_Port GPIOA
#define Motor3_DIR1_Pin GPIO_PIN_4
#define Motor3_DIR1_GPIO_Port GPIOC
#define LinearActuator_dir_Pin GPIO_PIN_5
#define LinearActuator_dir_GPIO_Port GPIOC
#define Encoder2_Pin GPIO_PIN_9
#define Encoder2_GPIO_Port GPIOE
#define Laser_pointer_Pin GPIO_PIN_11
#define Laser_pointer_GPIO_Port GPIOE
#define GPIO3_2_Pin GPIO_PIN_12
#define GPIO3_2_GPIO_Port GPIOE
#define laser1_Pin GPIO_PIN_13
#define laser1_GPIO_Port GPIOB
#define PassingMotor_DIR_Pin GPIO_PIN_9
#define PassingMotor_DIR_GPIO_Port GPIOD
#define LOCO_DIR2_Pin GPIO_PIN_11
#define LOCO_DIR2_GPIO_Port GPIOD
#define LOCO_DIR1_Pin GPIO_PIN_12
#define LOCO_DIR1_GPIO_Port GPIOD
#define LiftingMotor_DIR_Pin GPIO_PIN_13
#define LiftingMotor_DIR_GPIO_Port GPIOD
#define PassingMotor_PWM_Pin GPIO_PIN_14
#define PassingMotor_PWM_GPIO_Port GPIOD
#define LiftingMotor_PWM_Pin GPIO_PIN_15
#define LiftingMotor_PWM_GPIO_Port GPIOD
#define LinearActuator_EN_Pin GPIO_PIN_6
#define LinearActuator_EN_GPIO_Port GPIOC
#define LinearActuator_DIR1_Pin GPIO_PIN_7
#define LinearActuator_DIR1_GPIO_Port GPIOC
#define LinearActuator_DIR2_Pin GPIO_PIN_8
#define LinearActuator_DIR2_GPIO_Port GPIOC
#define LOCO_PWM1_Pin GPIO_PIN_6
#define LOCO_PWM1_GPIO_Port GPIOB
#define LOCO_PWM2_Pin GPIO_PIN_7
#define LOCO_PWM2_GPIO_Port GPIOB
#define STEPPER_STEP_Pin GPIO_PIN_8
#define STEPPER_STEP_GPIO_Port GPIOB
#define STEPPER_DIR_Pin GPIO_PIN_9
#define STEPPER_DIR_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
