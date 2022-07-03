/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
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
/* Includes ------------------------------------------------------------------*/
#include "main.h"

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
TIM_HandleTypeDef htim1;
TIM_HandleTypeDef htim2;
TIM_HandleTypeDef htim3;
TIM_HandleTypeDef htim4;
TIM_HandleTypeDef htim5;
TIM_HandleTypeDef htim9;
TIM_HandleTypeDef htim10;

UART_HandleTypeDef huart1;
DMA_HandleTypeDef hdma_usart1_rx;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
static void MX_USART1_UART_Init(void);
static void MX_TIM2_Init(void);
static void MX_TIM4_Init(void);
static void MX_TIM1_Init(void);
static void MX_TIM9_Init(void);
static void MX_TIM5_Init(void);
static void MX_TIM10_Init(void);
static void MX_TIM3_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
uint32_t IC_Value1 = 0;
uint32_t IC_Value2 = 0;
uint32_t Difference = 0;
uint32_t Frequency = 0;
uint8_t Is_First_Captured = 0;  // 0- not captured, 1- captured
uint32_t IC_Value12 = 0;
uint32_t IC_Value22 = 0;
uint32_t Difference2 = 0;
uint32_t Frequency2 = 0;
uint8_t Is_First_Captured2 = 0;  // 0- not captured, 1- captured
char rxData[1];
int start_stepper = 0;
int stepperIsOn = 0;
int countServo = 0;
int debounce = 0;
int checked = 0;
int stepperCount = 0;
int countBldc = 0;
int bldcDebounce = 0;
int linearDebounce = 5;
int dir1 = 0;
int dir2 = 0;
int laserr = 0;
int laserDebounce = 0;
int passMotor = 0;
int passDebounce = 0;
int countLas = 0;
int LasDebounce = 0;

///////////////////////////////////////////////////////////////////////////////////////////
//encoder feedback
void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim) {
//	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12, GPIO_PIN_SET);  //direction set

	if (htim == &htim1)  // if interrupt source is channel 1
			{
		if (Is_First_Captured == 0)  // is the first value captured ?
				{
			IC_Value1 = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_1); // capture the first value
			Is_First_Captured = 1;  // set the first value captured as true
		}

		else if (Is_First_Captured)  // if the first is captured
		{
			IC_Value2 = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_1); // capture second value

			if (IC_Value2 > IC_Value1) {
				Difference = IC_Value2 - IC_Value1;  // calculate the difference
			}

			else if (IC_Value2 < IC_Value1) {
				Difference = ((0xffff - IC_Value1) + IC_Value2) + 1;
			}

			else {
				Error_Handler();
			}

			Frequency = HAL_RCC_GetPCLK2Freq() / (50 * Difference); // calculate frequency

			Is_First_Captured = 0;  // reset the first captured
		}
	}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	if (htim == &htim2)  // if interrupt source is channel 1
			{
		if (Is_First_Captured2 == 0)  // is the first value captured ?
				{
			IC_Value12 = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_1); // capture the first value
			Is_First_Captured2 = 1;  // set the first value captured as true
		}

		else if (Is_First_Captured2)  // if the first is captured
		{
			IC_Value2 = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_1); // capture second value

			if (IC_Value22 > IC_Value12) {
				Difference2 = IC_Value2 - IC_Value12; // calculate the difference
			}

			else if (IC_Value22 < IC_Value12) {
				Difference2 = ((0xffff - IC_Value12) + IC_Value22) + 1;
			}

			else {
				Error_Handler();
			}

			Frequency2 = HAL_RCC_GetPCLK2Freq() / (50 * Difference2); // calculate frequency

			Is_First_Captured2 = 0;  // reset the first captured
		}
	}
}

///////////////////////////////////////////////////////////////////////////////////////////

class Loco {
public:
	TIM_HandleTypeDef *left_tim;
	TIM_HandleTypeDef *right_tim;
	uint32_t left_chnl;
	GPIO_TypeDef *left_p;
	GPIO_TypeDef *right_p;
	uint32_t right_chnl;
	uint16_t left_dir;
	uint16_t right_dir;
	uint16_t pwm;

	Loco(TIM_HandleTypeDef *leftm_tim, uint32_t leftm_channel,
			GPIO_TypeDef *left_port, uint16_t leftm_direction_pin,
			TIM_HandleTypeDef *rightm_tim, uint32_t rightm_channel,
			uint16_t rightm_direction_pin, GPIO_TypeDef *right_port,
			uint16_t pwmVal) {
		left_tim = leftm_tim;
		right_tim = rightm_tim;
		left_chnl = leftm_channel;
		right_chnl = rightm_channel;
		left_dir = leftm_direction_pin;
		right_dir = rightm_direction_pin;
		left_p = left_port;
		right_p = right_port;
		pwm = pwmVal;
	}

	void forward() {
		HAL_GPIO_WritePin(left_p, left_dir, GPIO_PIN_RESET);
				HAL_GPIO_WritePin(right_p, right_dir, GPIO_PIN_SET);
		__HAL_TIM_SET_COMPARE(left_tim, left_chnl, pwm);
		__HAL_TIM_SET_COMPARE(right_tim, right_chnl, pwm);
	}

	void backward() {
		HAL_GPIO_WritePin(left_p, left_dir, GPIO_PIN_SET);
		HAL_GPIO_WritePin(right_p, right_dir, GPIO_PIN_RESET);
		__HAL_TIM_SET_COMPARE(left_tim, left_chnl, pwm);
		__HAL_TIM_SET_COMPARE(right_tim, right_chnl, pwm);
	}

	void left() {
		HAL_GPIO_WritePin(left_p, left_dir, GPIO_PIN_SET);
		HAL_GPIO_WritePin(right_p, right_dir, GPIO_PIN_SET);
		__HAL_TIM_SET_COMPARE(left_tim, left_chnl, pwm);
		__HAL_TIM_SET_COMPARE(right_tim, right_chnl, pwm);
	}

	void right() {
		HAL_GPIO_WritePin(left_p, left_dir, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(right_p, right_dir, GPIO_PIN_RESET);
		__HAL_TIM_SET_COMPARE(left_tim, left_chnl, pwm);
		__HAL_TIM_SET_COMPARE(right_tim, right_chnl, pwm);
	}

	void stop() {
		HAL_GPIO_WritePin(left_p, left_dir, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(right_p, right_dir, GPIO_PIN_RESET);
		__HAL_TIM_SET_COMPARE(left_tim, left_chnl, 0);
		__HAL_TIM_SET_COMPARE(right_tim, right_chnl, 0);
	}
};

///////////////////////////////////////////////////////////////////////////////////////////////////
//utilities
void stepper() {
	HAL_GPIO_WritePin(GPIOB, STEPPER_DIR_Pin, GPIO_PIN_SET);
	while (stepperCount < 10) {
		__HAL_TIM_SET_COMPARE(&htim10, TIM_CHANNEL_1, 50);
	}
	start_stepper = 0;
	stepperCount = 0;
	__HAL_TIM_SET_COMPARE(&htim10, TIM_CHANNEL_1, 0);
}

void servo() {
	//90 degree rotation
	if (countServo == 0 && debounce == 0) {
		//__HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_4,50);
		__HAL_TIM_SET_COMPARE(&htim5, TIM_CHANNEL_3, 125); // duty cycle is 2.5 ms
		countServo = 1;
		debounce = 5;
	} else if (countServo == 1 && debounce == 0) {
		__HAL_TIM_SET_COMPARE(&htim5, TIM_CHANNEL_3, 50);
		countServo = 0;
		debounce = 5;
	}
}

void passingMotor() {
	if (passMotor == 0 && passDebounce == 0) {
		HAL_GPIO_WritePin(GPIOD, PassingMotor_DIR_Pin, GPIO_PIN_RESET);

		__HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_3, 50);
		passMotor = 1;
		passDebounce = 5;
	} else if (passMotor == 1 && passDebounce == 0) {
		HAL_GPIO_WritePin(GPIOD, PassingMotor_DIR_Pin, GPIO_PIN_RESET);
		__HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_3, 0);
		passMotor = 0;
		passDebounce = 5;
	}
}

void laser() {
	if (laserr == 0 && laserDebounce == 0) {
		HAL_GPIO_WritePin(GPIOB, laser1_Pin, GPIO_PIN_SET);

		laserr = 1;
		laserDebounce = 5;

	} else if (laserr == 1 && laserDebounce == 0) {
		HAL_GPIO_WritePin(GPIOB, laser1_Pin, GPIO_PIN_RESET);

		laserr = 0;
		laserDebounce = 5;
	}

}

void VerticalMotor(int dir) {
	if (dir == 0 && dir1 == 0 && linearDebounce == 0) {
		HAL_GPIO_WritePin(GPIOA, LinearActuator_en_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOC, LinearActuator_dir_Pin, GPIO_PIN_RESET);
		dir1 = 1;
		linearDebounce = 5;
	} else if (dir == 0 && dir1 == 1 && linearDebounce == 0) {
		HAL_GPIO_WritePin(GPIOA, LinearActuator_en_Pin, GPIO_PIN_RESET);
		dir1 = 0;
		linearDebounce = 5;
	}
	if (dir == 1 && dir2 == 0 && linearDebounce == 0) {
		HAL_GPIO_WritePin(GPIOA, LinearActuator_en_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOC, LinearActuator_dir_Pin, GPIO_PIN_SET);
		dir2 = 1;
		linearDebounce = 5;
	} else if (dir == 1 && dir2 == 1 && linearDebounce == 0) {
		HAL_GPIO_WritePin(GPIOA, LinearActuator_en_Pin, GPIO_PIN_RESET);

		dir2 = 0;
		linearDebounce = 5;
	}

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
	MX_DMA_Init();
	MX_USART1_UART_Init();
	MX_TIM2_Init();
	MX_TIM4_Init();
	MX_TIM1_Init();
	MX_TIM9_Init();
	MX_TIM5_Init();
	MX_TIM10_Init();
	MX_TIM3_Init();
	/* USER CODE BEGIN 2 */
	HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_1);  //Locomotion
	HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_2);  //Locomotion
	HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_3);  //Passing
	HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_4);  //Lifting
	HAL_TIM_PWM_Start(&htim5, TIM_CHANNEL_1);  //BLDC1
	HAL_TIM_PWM_Start(&htim5, TIM_CHANNEL_2);  //BLDC2
	HAL_TIM_PWM_Start(&htim5, TIM_CHANNEL_3);  //Servo
	HAL_TIM_PWM_Start(&htim10, TIM_CHANNEL_1);  //Stepper
	HAL_TIM_IC_Start_IT(&htim1, TIM_CHANNEL_1);  //Encoder
	HAL_TIM_IC_Start_IT(&htim2, TIM_CHANNEL_1);
	HAL_TIM_Base_Start_IT(&htim9);
	HAL_UART_Receive_DMA(&huart1, (uint8_t*) rxData, 1);

	Loco low = Loco(&htim4, TIM_CHANNEL_1, GPIOD, LOCO_DIR1_Pin, &htim4,
	TIM_CHANNEL_2, LOCO_DIR2_Pin, GPIOD, 60);
	Loco high = Loco(&htim4, TIM_CHANNEL_1, GPIOD, LOCO_DIR1_Pin, &htim4,
	TIM_CHANNEL_2, LOCO_DIR2_Pin, GPIOD, 80);
	__HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_4, 50);
	__HAL_TIM_SET_COMPARE(&htim5, TIM_CHANNEL_1, 0); //bldc default 030:94:35:32:df:3e
	__HAL_TIM_SET_COMPARE(&htim5, TIM_CHANNEL_2, 0);
	__HAL_TIM_SET_COMPARE(&htim5, TIM_CHANNEL_3, 50);

	HAL_GPIO_WritePin(GPIOE, Laser_pointer_Pin, GPIO_PIN_SET);
	////////////////////////////

	__HAL_TIM_SET_COMPARE(&htim5, TIM_CHANNEL_2, 100);
	__HAL_TIM_SET_COMPARE(&htim10, TIM_CHANNEL_1, 0);
	/* USER CODE END 2 */

	/* Infinite loop */
	/* USER CODE BEGIN WHILE */
	while (1) {
		/* USER CODE END WHILE */

		/* USER CODE BEGIN 3 */
		if (rxData[0] == 13 && laserDebounce == 0) {
			laser();

		}
		if (rxData[0] == 1) //up//Button Up
				{
			low.forward();
		}

		//Down // Button Down
		else if (rxData[0] == 2) {
			low.backward();
		}

		//left // Button Left
		else if (rxData[0] == 3) {
			low.left();
		}

		//right // Button Rights
		else if (rxData[0] == 4) {
			low.right();
		} else if (rxData[0] == 14) //up//Button Up
				{
			high.forward();
		}

		//Down // Button Down
		else if (rxData[0] == 16) {
			high.backward();
		}

		//left // Button Left
		else if (rxData[0] == 20) {
			high.left();
		}

		//right // Button Rights
		else if (rxData[0] == 18) {
			high.right();
		} else if (rxData[0] == 15) //up//Button Up
				{
			low.forward();
		}

		//Down // Button Down
		else if (rxData[0] == 17) {
			low.backward();
		}

		//left // Button Left
		else if (rxData[0] == 21) {
			low.left();
		}

		//right // Button Rights
		else if (rxData[0] == 19) {
			low.right();
		}
		//PASSING //Button Square
		else if (rxData[0] == 5) {
			//Passing on
			passingMotor();

		}

		//toggle servo//Button Circle
		else if (rxData[0] == 6) {
			servo();

		}

		//toggle linear actuator down  //Button L1
		else if (rxData[0] == 7) {
			VerticalMotor(1);

		}

		//toggle linear actuator up	  //Button R1
		else if (rxData[0] == 9) { //&& !HAL_GPIO_ReadPin(GPIOE, Limit_switch_input_Pin) {
			VerticalMotor(0);
		}

		//BLDC ON //Button L2
		else if (rxData[0] == 8) {

			if (countBldc == 0 && bldcDebounce == 0) {
				HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, GPIO_PIN_SET);
				HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, GPIO_PIN_SET);
				countBldc = 1;
				bldcDebounce = 5;
			} else if (countBldc == 1 && bldcDebounce == 0) {
				HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, GPIO_PIN_RESET);
				HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, GPIO_PIN_RESET);
				countBldc = 0;
				bldcDebounce = 5;
			}

		}

		// Stepper 90 deg//Button R2
		else if (rxData[0] == 10) {
			start_stepper = 1;
			stepper();
		}
		//Lifting Up //Button Triangle
		else if (rxData[0] == 11){// && !HAL_GPIO_ReadPin(GPIOE, Limit_switch_input1_Pin)) {
			HAL_GPIO_WritePin(GPIOD, LiftingMotor_DIR_Pin, GPIO_PIN_SET);
			__HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_4, 70);
		}
		//Lifting Down// Button Cross
		else if (rxData[0] == 12){// && !HAL_GPIO_ReadPin(GPIOE, Liimit_switch_input2_Pin)) {
			HAL_GPIO_WritePin(GPIOD, LiftingMotor_DIR_Pin, GPIO_PIN_RESET);
			__HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_4, 70);

		} else {
			low.stop();
			//			HAL_GPIO_WritePin(GPIOD, GPIO_PIN_14, GPIO_PIN_RESET);
			//			HAL_GPIO_WritePin(GPIOD, GPIO_PIN_15, GPIO_PIN_RESET);
			__HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_4, 0); //lifting 0
			__HAL_TIM_SET_COMPARE(&htim5, TIM_CHANNEL_1, 0); //BLDC 0
			__HAL_TIM_SET_COMPARE(&htim5, TIM_CHANNEL_2, 0); //BLDC 0
			__HAL_TIM_SET_COMPARE(&htim10, TIM_CHANNEL_1, 0); //stepper to zero

		}
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
	RCC_OscInitStruct.PLL.PLLN = 50;
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
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
	RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

	if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK) {
		Error_Handler();
	}
}

/**
 * @brief TIM1 Initialization Function
 * @param None
 * @retval None
 */
static void MX_TIM1_Init(void) {

	/* USER CODE BEGIN TIM1_Init 0 */

	/* USER CODE END TIM1_Init 0 */

	TIM_ClockConfigTypeDef sClockSourceConfig = { 0 };
	TIM_MasterConfigTypeDef sMasterConfig = { 0 };
	TIM_IC_InitTypeDef sConfigIC = { 0 };

	/* USER CODE BEGIN TIM1_Init 1 */

	/* USER CODE END TIM1_Init 1 */
	htim1.Instance = TIM1;
	htim1.Init.Prescaler = 0;
	htim1.Init.CounterMode = TIM_COUNTERMODE_UP;
	htim1.Init.Period = 65535;
	htim1.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	htim1.Init.RepetitionCounter = 0;
	htim1.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
	if (HAL_TIM_Base_Init(&htim1) != HAL_OK) {
		Error_Handler();
	}
	sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
	if (HAL_TIM_ConfigClockSource(&htim1, &sClockSourceConfig) != HAL_OK) {
		Error_Handler();
	}
	if (HAL_TIM_IC_Init(&htim1) != HAL_OK) {
		Error_Handler();
	}
	sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
	sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
	if (HAL_TIMEx_MasterConfigSynchronization(&htim1, &sMasterConfig)
			!= HAL_OK) {
		Error_Handler();
	}
	sConfigIC.ICPolarity = TIM_INPUTCHANNELPOLARITY_RISING;
	sConfigIC.ICSelection = TIM_ICSELECTION_DIRECTTI;
	sConfigIC.ICPrescaler = TIM_ICPSC_DIV1;
	sConfigIC.ICFilter = 0;
	if (HAL_TIM_IC_ConfigChannel(&htim1, &sConfigIC, TIM_CHANNEL_1) != HAL_OK) {
		Error_Handler();
	}
	/* USER CODE BEGIN TIM1_Init 2 */

	/* USER CODE END TIM1_Init 2 */

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
	TIM_IC_InitTypeDef sConfigIC = { 0 };

	/* USER CODE BEGIN TIM2_Init 1 */

	/* USER CODE END TIM2_Init 1 */
	htim2.Instance = TIM2;
	htim2.Init.Prescaler = 0;
	htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
	htim2.Init.Period = 4294967295;
	htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
	if (HAL_TIM_Base_Init(&htim2) != HAL_OK) {
		Error_Handler();
	}
	sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
	if (HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig) != HAL_OK) {
		Error_Handler();
	}
	if (HAL_TIM_IC_Init(&htim2) != HAL_OK) {
		Error_Handler();
	}
	sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
	sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
	if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig)
			!= HAL_OK) {
		Error_Handler();
	}
	sConfigIC.ICPolarity = TIM_INPUTCHANNELPOLARITY_RISING;
	sConfigIC.ICSelection = TIM_ICSELECTION_DIRECTTI;
	sConfigIC.ICPrescaler = TIM_ICPSC_DIV1;
	sConfigIC.ICFilter = 0;
	if (HAL_TIM_IC_ConfigChannel(&htim2, &sConfigIC, TIM_CHANNEL_1) != HAL_OK) {
		Error_Handler();
	}
	/* USER CODE BEGIN TIM2_Init 2 */

	/* USER CODE END TIM2_Init 2 */

}

/**
 * @brief TIM3 Initialization Function
 * @param None
 * @retval None
 */
static void MX_TIM3_Init(void) {

	/* USER CODE BEGIN TIM3_Init 0 */

	/* USER CODE END TIM3_Init 0 */

	TIM_MasterConfigTypeDef sMasterConfig = { 0 };
	TIM_OC_InitTypeDef sConfigOC = { 0 };

	/* USER CODE BEGIN TIM3_Init 1 */

	/* USER CODE END TIM3_Init 1 */
	htim3.Instance = TIM3;
	htim3.Init.Prescaler = 0;
	htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
	htim3.Init.Period = 65535;
	htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
	if (HAL_TIM_PWM_Init(&htim3) != HAL_OK) {
		Error_Handler();
	}
	sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
	sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
	if (HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig)
			!= HAL_OK) {
		Error_Handler();
	}
	sConfigOC.OCMode = TIM_OCMODE_PWM1;
	sConfigOC.Pulse = 0;
	sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
	sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
	if (HAL_TIM_PWM_ConfigChannel(&htim3, &sConfigOC, TIM_CHANNEL_1)
			!= HAL_OK) {
		Error_Handler();
	}
	/* USER CODE BEGIN TIM3_Init 2 */

	/* USER CODE END TIM3_Init 2 */
	HAL_TIM_MspPostInit(&htim3);

}

/**
 * @brief TIM4 Initialization Function
 * @param None
 * @retval None
 */
static void MX_TIM4_Init(void) {

	/* USER CODE BEGIN TIM4_Init 0 */

	/* USER CODE END TIM4_Init 0 */

	TIM_MasterConfigTypeDef sMasterConfig = { 0 };
	TIM_OC_InitTypeDef sConfigOC = { 0 };

	/* USER CODE BEGIN TIM4_Init 1 */

	/* USER CODE END TIM4_Init 1 */
	htim4.Instance = TIM4;
	htim4.Init.Prescaler = 1000;
	htim4.Init.CounterMode = TIM_COUNTERMODE_UP;
	htim4.Init.Period = 100;
	htim4.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	htim4.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
	if (HAL_TIM_PWM_Init(&htim4) != HAL_OK) {
		Error_Handler();
	}
	sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
	sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
	if (HAL_TIMEx_MasterConfigSynchronization(&htim4, &sMasterConfig)
			!= HAL_OK) {
		Error_Handler();
	}
	sConfigOC.OCMode = TIM_OCMODE_PWM1;
	sConfigOC.Pulse = 0;
	sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
	sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
	if (HAL_TIM_PWM_ConfigChannel(&htim4, &sConfigOC, TIM_CHANNEL_1)
			!= HAL_OK) {
		Error_Handler();
	}
	if (HAL_TIM_PWM_ConfigChannel(&htim4, &sConfigOC, TIM_CHANNEL_2)
			!= HAL_OK) {
		Error_Handler();
	}
	if (HAL_TIM_PWM_ConfigChannel(&htim4, &sConfigOC, TIM_CHANNEL_3)
			!= HAL_OK) {
		Error_Handler();
	}
	if (HAL_TIM_PWM_ConfigChannel(&htim4, &sConfigOC, TIM_CHANNEL_4)
			!= HAL_OK) {
		Error_Handler();
	}
	/* USER CODE BEGIN TIM4_Init 2 */

	/* USER CODE END TIM4_Init 2 */
	HAL_TIM_MspPostInit(&htim4);

}

/**
 * @brief TIM5 Initialization Function
 * @param None
 * @retval None
 */
static void MX_TIM5_Init(void) {

	/* USER CODE BEGIN TIM5_Init 0 */

	/* USER CODE END TIM5_Init 0 */

	TIM_ClockConfigTypeDef sClockSourceConfig = { 0 };
	TIM_MasterConfigTypeDef sMasterConfig = { 0 };
	TIM_OC_InitTypeDef sConfigOC = { 0 };

	/* USER CODE BEGIN TIM5_Init 1 */

	/* USER CODE END TIM5_Init 1 */
	htim5.Instance = TIM5;
	htim5.Init.Prescaler = 1000 - 1;
	htim5.Init.CounterMode = TIM_COUNTERMODE_UP;
	htim5.Init.Period = 1000 - 1;
	htim5.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	htim5.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
	if (HAL_TIM_Base_Init(&htim5) != HAL_OK) {
		Error_Handler();
	}
	sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
	if (HAL_TIM_ConfigClockSource(&htim5, &sClockSourceConfig) != HAL_OK) {
		Error_Handler();
	}
	if (HAL_TIM_PWM_Init(&htim5) != HAL_OK) {
		Error_Handler();
	}
	sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
	sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
	if (HAL_TIMEx_MasterConfigSynchronization(&htim5, &sMasterConfig)
			!= HAL_OK) {
		Error_Handler();
	}
	sConfigOC.OCMode = TIM_OCMODE_PWM1;
	sConfigOC.Pulse = 0;
	sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
	sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
	if (HAL_TIM_PWM_ConfigChannel(&htim5, &sConfigOC, TIM_CHANNEL_3)
			!= HAL_OK) {
		Error_Handler();
	}
	/* USER CODE BEGIN TIM5_Init 2 */

	/* USER CODE END TIM5_Init 2 */
	HAL_TIM_MspPostInit(&htim5);

}

/**
 * @brief TIM9 Initialization Function
 * @param None
 * @retval None
 */
static void MX_TIM9_Init(void) {

	/* USER CODE BEGIN TIM9_Init 0 */

	/* USER CODE END TIM9_Init 0 */

	TIM_ClockConfigTypeDef sClockSourceConfig = { 0 };

	/* USER CODE BEGIN TIM9_Init 1 */

	/* USER CODE END TIM9_Init 1 */
	htim9.Instance = TIM9;
	htim9.Init.Prescaler = 50000 - 1;
	htim9.Init.CounterMode = TIM_COUNTERMODE_UP;
	htim9.Init.Period = 100;
	htim9.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	htim9.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
	if (HAL_TIM_Base_Init(&htim9) != HAL_OK) {
		Error_Handler();
	}
	sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
	if (HAL_TIM_ConfigClockSource(&htim9, &sClockSourceConfig) != HAL_OK) {
		Error_Handler();
	}
	/* USER CODE BEGIN TIM9_Init 2 */

	/* USER CODE END TIM9_Init 2 */

}

/**
 * @brief TIM10 Initialization Function
 * @param None
 * @retval None
 */
static void MX_TIM10_Init(void) {

	/* USER CODE BEGIN TIM10_Init 0 */

	/* USER CODE END TIM10_Init 0 */

	TIM_OC_InitTypeDef sConfigOC = { 0 };

	/* USER CODE BEGIN TIM10_Init 1 */

	/* USER CODE END TIM10_Init 1 */
	htim10.Instance = TIM10;
	htim10.Init.Prescaler = 1000 - 1;
	htim10.Init.CounterMode = TIM_COUNTERMODE_UP;
	htim10.Init.Period = 1000 - 1;
	htim10.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	htim10.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
	if (HAL_TIM_Base_Init(&htim10) != HAL_OK) {
		Error_Handler();
	}
	if (HAL_TIM_PWM_Init(&htim10) != HAL_OK) {
		Error_Handler();
	}
	sConfigOC.OCMode = TIM_OCMODE_PWM1;
	sConfigOC.Pulse = 0;
	sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
	sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
	if (HAL_TIM_PWM_ConfigChannel(&htim10, &sConfigOC, TIM_CHANNEL_1)
			!= HAL_OK) {
		Error_Handler();
	}
	/* USER CODE BEGIN TIM10_Init 2 */

	/* USER CODE END TIM10_Init 2 */
	HAL_TIM_MspPostInit(&htim10);

}

/**
 * @brief USART1 Initialization Function
 * @param None
 * @retval None
 */
static void MX_USART1_UART_Init(void) {

	/* USER CODE BEGIN USART1_Init 0 */

	/* USER CODE END USART1_Init 0 */

	/* USER CODE BEGIN USART1_Init 1 */

	/* USER CODE END USART1_Init 1 */
	huart1.Instance = USART1;
	huart1.Init.BaudRate = 115200;
	huart1.Init.WordLength = UART_WORDLENGTH_8B;
	huart1.Init.StopBits = UART_STOPBITS_1;
	huart1.Init.Parity = UART_PARITY_NONE;
	huart1.Init.Mode = UART_MODE_TX_RX;
	huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	huart1.Init.OverSampling = UART_OVERSAMPLING_16;
	if (HAL_UART_Init(&huart1) != HAL_OK) {
		Error_Handler();
	}
	/* USER CODE BEGIN USART1_Init 2 */

	/* USER CODE END USART1_Init 2 */

}

/**
 * Enable DMA controller clock
 */
static void MX_DMA_Init(void) {

	/* DMA controller clock enable */
	__HAL_RCC_DMA2_CLK_ENABLE();

	/* DMA interrupt init */
	/* DMA2_Stream2_IRQn interrupt configuration */
	HAL_NVIC_SetPriority(DMA2_Stream2_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(DMA2_Stream2_IRQn);

}

/**
 * @brief GPIO Initialization Function
 * @param None
 * @retval None
 */
static void MX_GPIO_Init(void) {
	GPIO_InitTypeDef GPIO_InitStruct = { 0 };

	/* GPIO Ports Clock Enable */
	__HAL_RCC_GPIOE_CLK_ENABLE();
	__HAL_RCC_GPIOC_CLK_ENABLE();
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();
	__HAL_RCC_GPIOD_CLK_ENABLE();

	/*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(GPIOE,
			Laser1_en_Pin | GPIO2_2_Pin | Laser_pointer_Pin | GPIO3_2_Pin,
			GPIO_PIN_RESET);

	/*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(GPIOC,
			BLDC_1_Pin | BLDC_2_Pin | Motor3_DIR1_Pin | LinearActuator_dir_Pin
					| LinearActuator_EN_Pin | LinearActuator_DIR1_Pin
					| LinearActuator_DIR2_Pin, GPIO_PIN_RESET);

	/*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0 | GPIO_PIN_1 | LinearActuator_en_Pin,
			GPIO_PIN_RESET);

	/*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(GPIOB, laser1_Pin | STEPPER_DIR_Pin, GPIO_PIN_RESET);

	/*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(GPIOD,
			PassingMotor_DIR_Pin | LOCO_DIR2_Pin | LOCO_DIR1_Pin
					| LiftingMotor_DIR_Pin, GPIO_PIN_RESET);

	/*Configure GPIO pins : Limit_switch_input1_Pin Liimit_switch_input2_Pin */
	GPIO_InitStruct.Pin = Limit_switch_input1_Pin | Liimit_switch_input2_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

	/*Configure GPIO pins : Laser1_en_Pin GPIO2_2_Pin Laser_pointer_Pin GPIO3_2_Pin */
	GPIO_InitStruct.Pin = Laser1_en_Pin | GPIO2_2_Pin | Laser_pointer_Pin
			| GPIO3_2_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

	/*Configure GPIO pins : BLDC_1_Pin BLDC_2_Pin Motor3_DIR1_Pin LinearActuator_dir_Pin
	 LinearActuator_EN_Pin LinearActuator_DIR1_Pin LinearActuator_DIR2_Pin */
	GPIO_InitStruct.Pin = BLDC_1_Pin | BLDC_2_Pin | Motor3_DIR1_Pin
			| LinearActuator_dir_Pin | LinearActuator_EN_Pin
			| LinearActuator_DIR1_Pin | LinearActuator_DIR2_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

	/*Configure GPIO pins : PA0 PA1 LinearActuator_en_Pin */
	GPIO_InitStruct.Pin = GPIO_PIN_0 | GPIO_PIN_1 | LinearActuator_en_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

	/*Configure GPIO pins : laser1_Pin STEPPER_DIR_Pin */
	GPIO_InitStruct.Pin = laser1_Pin | STEPPER_DIR_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

	/*Configure GPIO pins : PassingMotor_DIR_Pin LOCO_DIR2_Pin LOCO_DIR1_Pin LiftingMotor_DIR_Pin */
	GPIO_InitStruct.Pin = PassingMotor_DIR_Pin | LOCO_DIR2_Pin | LOCO_DIR1_Pin
			| LiftingMotor_DIR_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
	//test +=1;

	if (laserDebounce > 0) {
		laserDebounce--;
	}
	if (passDebounce > 0) {
		passDebounce--;
	}

	if (linearDebounce > 0) {
		linearDebounce--;

	}
	if (bldcDebounce > 0) {
		bldcDebounce--;
	}
	if (debounce > 0) {
		debounce--;
	}
	if (start_stepper) {
		stepperCount += 1;
	}

}
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

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
