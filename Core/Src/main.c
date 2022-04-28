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
char rxData[3];
int lPID = 0;
int rPID = 0;
int lFlag = 0;
int rFlag = 0;
int flag = 0;
int check = 0;
uint32_t IC_Value1 = 0;
uint32_t IC_Value2 = 0;
uint32_t Difference = 0;
uint32_t Frequency = 0;
int chee =0;
uint8_t Is_First_Captured = 0;  // 0- not captured, 1- captured

uint32_t IC_Value12 = 0;
uint32_t IC_Value22 = 0;
uint32_t Difference2 = 0;
uint32_t Frequency2 = 0;
uint8_t Is_First_Captured2 = 0;  // 0- not captured, 1- captured
int PID = 0;
uint32_t errorSum = 0;
int target = 500;
float KI = 0.5;
float KP = 2;
float KD = 3;
int speed=0;
int speed2 = 0;
int prev_speed=0;
int start_stepper = 0;
int stepperIsOn = 0;
int countServo = 0;
int debounce = 0;
int checked = 0;
int stepperCount = 0;
int countBldc = 0;
int bldcDebounce = 0;
int linearDebounce=5;
int dir1=0;
int dir2=0;
int laserr = 0;
int laserDebounce = 0;
int passMotor = 0;
int passDebounce = 0;
//if PWM is not working then use max 40kHz of frequency for cytron driver


void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
//	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12, GPIO_PIN_SET);  //direction set

 if (htim == &htim1)  // if interrupt source is channel 1
 {
  if (Is_First_Captured==0)  // is the first value captured ?
  {
   IC_Value1 = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_1);  // capture the first value
   Is_First_Captured =1;  // set the first value captured as true
  }

  else if (Is_First_Captured)  // if the first is captured
  {
   IC_Value2 = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_1);  // capture second value

   if (IC_Value2 > IC_Value1)
   {
    Difference = IC_Value2-IC_Value1;   // calculate the difference
   }

   else if (IC_Value2 < IC_Value1)
   {
    Difference = ((0xffff-IC_Value1)+IC_Value2) +1;
   }

   else
   {
    Error_Handler();
   }

   Frequency = HAL_RCC_GetPCLK2Freq()/(50*Difference);  // calculate frequency


   Is_First_Captured = 0;  // reset the first captured
  }
 }

////////////////////////////////////////////////////////////////////////////////////////////////////////////////

 if (htim == &htim2)  // if interrupt source is channel 1
  {
   if (Is_First_Captured2==0)  // is the first value captured ?
   {
    IC_Value12 = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_1);  // capture the first value
    Is_First_Captured2 =1;  // set the first value captured as true
   }

   else if (Is_First_Captured2)  // if the first is captured
   {
    IC_Value2 = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_1);  // capture second value

    if (IC_Value22 > IC_Value12)
    {
     Difference2 = IC_Value2-IC_Value12;   // calculate the difference
    }

    else if (IC_Value22 < IC_Value12)
    {
     Difference2 = ((0xffff-IC_Value12)+IC_Value22) +1;
    }

    else
    {
     Error_Handler();
    }

    Frequency2 = HAL_RCC_GetPCLK2Freq()/(50*Difference2);  // calculate frequency

    Is_First_Captured2 = 0;  // reset the first captured
   }
  }
}

void stepper(){
	HAL_GPIO_WritePin(GPIOB,STEPPER_DIR_Pin,GPIO_PIN_SET);
	while(stepperCount<10){
		__HAL_TIM_SET_COMPARE(&htim10, TIM_CHANNEL_1,50);
	}
	start_stepper=0;
	stepperCount=0;
	__HAL_TIM_SET_COMPARE(&htim10, TIM_CHANNEL_1,0);
}

void servo(){
	//90 degree rotation
	if(countServo==0 && debounce==0){
		//__HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_4,50);
		__HAL_TIM_SET_COMPARE(&htim5, TIM_CHANNEL_3,125);  // duty cycle is 2.5 ms
	countServo=1;
	debounce =5;
	}else if(countServo==1 && debounce==0){
		__HAL_TIM_SET_COMPARE(&htim5, TIM_CHANNEL_3,50);
			countServo=0;
			debounce =5;
	}
}

void passingMotor(){
	if(passMotor == 0 && passDebounce == 0){
		HAL_GPIO_WritePin(GPIOD, PassingMotor_DIR_Pin, GPIO_PIN_SET);

			  	  	 	__HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_3,32);
			  	  	 	passMotor=1;
			  	  	 	passDebounce =5;
	}else if(passMotor == 1 && passDebounce == 0){
		HAL_GPIO_WritePin(GPIOD, PassingMotor_DIR_Pin, GPIO_PIN_SET);
					  	  	 	__HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_3,0);
					  	  	 	passMotor=0;
					  	  	 	passDebounce = 5;
	}
}


void laser(){
	if(laserr==0 && laserDebounce==0){
		HAL_GPIO_WritePin(GPIOE, Laser_pointer_Pin, GPIO_PIN_SET);
		laserr = 1;
		laserDebounce = 5;
	}else if(laserr==1 && laserDebounce==0){
		HAL_GPIO_WritePin(GPIOE, Laser_pointer_Pin, GPIO_PIN_RESET);
		laserr = 0;
		laserDebounce = 5;
	}

}

void VerticalMotor(int dir){
	if(dir== 0 && dir1 == 0 && linearDebounce==0){
		 HAL_GPIO_WritePin(GPIOA, LinearActuator_en_Pin, GPIO_PIN_SET);
		 HAL_GPIO_WritePin(GPIOC, LinearActuator_dir_Pin, GPIO_PIN_RESET);
		 dir1 = 1;
		 linearDebounce= 5;
	}else if(dir ==0 && dir1 == 1 && linearDebounce==0){
		 HAL_GPIO_WritePin(GPIOA, LinearActuator_en_Pin, GPIO_PIN_RESET);
				 dir1=0;
				 linearDebounce=5;
	}
	if(dir ==1 && dir2==0 && linearDebounce==0){
		HAL_GPIO_WritePin(GPIOA, LinearActuator_en_Pin, GPIO_PIN_SET);
		 HAL_GPIO_WritePin(GPIOC, LinearActuator_dir_Pin, GPIO_PIN_SET);
		 dir2 = 1;
		 linearDebounce=5;
	}else if(dir == 1 && dir2==1 && linearDebounce==0){
		HAL_GPIO_WritePin(GPIOA, LinearActuator_en_Pin, GPIO_PIN_RESET);

		 dir2 = 0;
		 linearDebounce=5;
	}

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
  HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_1);//Locomotion
  HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_2);//Locomotion

	HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_3);//Passing
	HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_4);//Lifting

	 HAL_TIM_PWM_Start(&htim5, TIM_CHANNEL_1);//BLDC1
	 HAL_TIM_PWM_Start(&htim5, TIM_CHANNEL_2);//BLDC2
	 HAL_TIM_PWM_Start(&htim5, TIM_CHANNEL_3);//Servo

	 HAL_TIM_PWM_Start(&htim10, TIM_CHANNEL_1);//Stepper

	HAL_TIM_IC_Start_IT(&htim1, TIM_CHANNEL_1);//Encoder
	HAL_TIM_IC_Start_IT(&htim2, TIM_CHANNEL_1);

	HAL_TIM_Base_Start_IT(&htim9);
	HAL_UART_Receive_DMA(&huart1, (uint8_t *)rxData, 3);
	__HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_4,50);
	__HAL_TIM_SET_COMPARE(&htim5, TIM_CHANNEL_1,0);//bldc default 030:94:35:32:df:3e
	__HAL_TIM_SET_COMPARE(&htim5, TIM_CHANNEL_2,0);
	__HAL_TIM_SET_COMPARE(&htim5, TIM_CHANNEL_3,50);

	HAL_GPIO_WritePin(GPIOE, Laser_pointer_Pin, GPIO_PIN_SET);
	////////////////////////////

	__HAL_TIM_SET_COMPARE(&htim5, TIM_CHANNEL_2,100);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
	  if(flag){
	  	  	  		speed = Frequency/30;
	  	  	  		speed2 = Frequency2/30;
	  	  	  		errorSum = errorSum + (target - speed)*0.1;
	  	  	  	PID = (target-speed)*KP + KD*(speed - prev_speed)/0.1;// + KI*errorSum;
	  	  	  		  if(PID<0){
	  	  	  			  PID = 0;
	  	  	  		  }else if(PID>100){
	  	  	  			  PID = 100;
	  	  	  		  }
	  	  	  		  //__HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_1,PID);// PID*100/target);
	  	  	  		  prev_speed = speed;
	  	  	  		  flag = 0;
	  	  	  	  }

	  	  if(rxData[0] == 1) //up//Button Up
	  	  		{
	  		  HAL_GPIO_WritePin(GPIOD, LOCO_DIR1_Pin, GPIO_PIN_RESET);
	  		  HAL_GPIO_WritePin(GPIOD, LOCO_DIR2_Pin, GPIO_PIN_SET);

	  		  __HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_1,100);
	  		  __HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_2,100);
	  	  		}

	  	  //Down // Button Down
	  	  		else if (rxData[0] == 2)
	  	  		{
	  	  		  HAL_GPIO_WritePin(GPIOD, LOCO_DIR1_Pin, GPIO_PIN_SET);
	  	  		  HAL_GPIO_WritePin(GPIOD, LOCO_DIR2_Pin, GPIO_PIN_RESET);

	  	  		__HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_1,100);
	  	  		__HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_2,100);
	  	  		}

	  	  		//left // Button Left
	  	  		else if (rxData[0] == 3)
	  	  		{
	  	  			HAL_GPIO_WritePin(GPIOD, LOCO_DIR1_Pin, GPIO_PIN_SET);
	  	  			HAL_GPIO_WritePin(GPIOD, LOCO_DIR2_Pin, GPIO_PIN_SET);

	  	  		    __HAL_TIM_SET_COMPARE(&htim4,TIM_CHANNEL_1,100);
	  		        __HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_2,100);
	  	    }

	  	  	  //right // Button Rights
	  	  		else if (rxData[0] == 4)
	  	  		{
	  	  			HAL_GPIO_WritePin(GPIOD, LOCO_DIR1_Pin, GPIO_PIN_RESET);
				    HAL_GPIO_WritePin(GPIOD, LOCO_DIR2_Pin, GPIO_PIN_RESET);

					__HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_1,100);
				    __HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_2,100);
	  	    }

	  	  	  //PASSING //Button Square
	  	  	else if (rxData[0] == 5)
	  	  	{
	  	  		//Passing on
	  	  		passingMotor();
//
//	  	  	if(countBldc==0 && bldcDebounce == 0){
//	  	  					__HAL_TIM_SET_COMPARE(&htim5, TIM_CHANNEL_1,1000);
//	  	  					__HAL_TIM_SET_COMPARE(&htim5, TIM_CHANNEL_2,1000);
//	  	  						countBldc=1;
//	  	  						bldcDebounce = 5;
//	  	  						}
//	  	  					else if(countBldc==1 && bldcDebounce == 0){
//	  	  						__HAL_TIM_SET_COMPARE(&htim5, TIM_CHANNEL_1,0);
//	  	  						__HAL_TIM_SET_COMPARE(&htim5, TIM_CHANNEL_2,0);
//	  	  						countBldc=0;
//	  	  					bldcDebounce = 5;
//	  	  		  	  		}
	  	  	}

	  	 //toggle servo//Button Circle
	  	  	else if (rxData[0] == 6)
	  	  	 {
	  	  	servo();

	  	  	 }

	  	    //toggle linear actuator down  //Button L1
	  	  	else if(rxData[0] == 7){
	  	  	// HAL_GPIO_WritePin(GPIOC, LinearActuator_DIR1_Pin, GPIO_PIN_SET);
			 //HAL_GPIO_WritePin(GPIOC, LinearActuator_DIR2_Pin, GPIO_PIN_SET);
				VerticalMotor(1);
	  			 //HAL_GPIO_WritePin(GPIOA, LinearActuator_en_Pin, GPIO_PIN_SET);
	  			 //HAL_GPIO_WritePin(GPIOC, LinearActuator_dir_Pin, GPIO_PIN_RESET);
	  	  	}

			//toggle linear actuator up	  //Button R1
	  	  	else if(rxData[0] == 9){
	  	  	 //HAL_GPIO_WritePin(GPIOC, LinearActuator_DIR1_Pin, GPIO_PIN_SET);
	  	  		//	 HAL_GPIO_WritePin(GPIOC, LinearActuator_DIR2_Pin, GPIO_PIN_RESET);
	  	  			 VerticalMotor(0);
	  	  		// HAL_GPIO_WritePin(GPIOA, LinearActuator_en_Pin, GPIO_PIN_SET);
	  	  		// HAL_GPIO_WritePin(GPIOC, LinearActuator_dir_Pin, GPIO_PIN_SET);
	  	  	}

	  	   //BLDC ON //Button L2
	  	  	else if(rxData[0] == 8){
//	  	  	 HAL_GPIO_WritePin(GPIOA, BLDC_PWM1_Pin, GPIO_PIN_SET);
//	  	  	 HAL_GPIO_WritePin(GPIOA, BLDC_PWM2_Pin, GPIO_PIN_SET);
				if(countBldc==0 && bldcDebounce == 0){
					HAL_GPIO_WritePin(GPIOC, BLDC_1_Pin, GPIO_PIN_SET);
					HAL_GPIO_WritePin(GPIOC, BLDC_2_Pin, GPIO_PIN_SET);
					countBldc=1;
					bldcDebounce = 5;
					}
				else if(countBldc==1 && bldcDebounce == 0){
					HAL_GPIO_WritePin(GPIOC, BLDC_2_Pin, GPIO_PIN_RESET);
				 HAL_GPIO_WritePin(GPIOC, BLDC_1_Pin, GPIO_PIN_RESET);
					countBldc=0;
				bldcDebounce = 5;
	  	  		}
//	  	  	__HAL_TIM_SET_COMPARE(&htim5, TIM_CHANNEL_1,1000);
//	  	  	__HAL_TIM_SET_COMPARE(&htim5, TIM_CHANNEL_2,1000);
	  	  	}

	  	  // Stepper 90 deg//Button R2
	  	  	else if(rxData[0] == 10){
				start_stepper = 1;
				stepper();
	  	  	}
	  	  	  //Lifting Up //Button Triangle
	  	  	else if(rxData[0] == 11 && !HAL_GPIO_ReadPin (GPIOE, Limit_switch_input_Pin)){ //added limit switch to pulling up
				 HAL_GPIO_WritePin(GPIOD, LiftingMotor_DIR_Pin, GPIO_PIN_SET);

				 __HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_4,250);
	  	  	}
	  	  	  //Lifting Down// Button Cross
	  	  	else if(rxData[0] == 12){
	  	  	  	  HAL_GPIO_WritePin(GPIOD,  LiftingMotor_DIR_Pin, GPIO_PIN_RESET);

				  __HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_4,250);
	  	  	}
	  	  	else{
	  	  	  	  		  	//repeating block to set PID frequency and setting tp zero
//	  	  	  	  	  check = 0;
//	  	  	  	  	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_7, GPIO_PIN_RESET);
//	  	  	  	  		  	  	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_8, GPIO_PIN_RESET);
	  	  	HAL_GPIO_WritePin(GPIOC, LinearActuator_DIR1_Pin, GPIO_PIN_RESET); //linearActuator
	  	  	  	  		  	__HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_1,0);
	  	  	  	  		  	__HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_2,0);

	  	  	  	  		  	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_14,GPIO_PIN_RESET );
	  	  	  	  		  	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_15,GPIO_PIN_RESET );

	  	  	  	  	 __HAL_TIM_SET_COMPARE(&htim4,TIM_CHANNEL_1,0);//locomotion 0
	  	  	  	  	 __HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_2,0);


	  	  	  	  		__HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_4,0);//lifting 0

	  	  	  	  __HAL_TIM_SET_COMPARE(&htim5, TIM_CHANNEL_1,0);//BLDC 0
	  	  	  	  	  	  	__HAL_TIM_SET_COMPARE(&htim5, TIM_CHANNEL_2,0);//BLDC 0


	  	  	  	  		  	    }
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
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief TIM1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM1_Init(void)
{

  /* USER CODE BEGIN TIM1_Init 0 */

  /* USER CODE END TIM1_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_IC_InitTypeDef sConfigIC = {0};

  /* USER CODE BEGIN TIM1_Init 1 */

  /* USER CODE END TIM1_Init 1 */
  htim1.Instance = TIM1;
  htim1.Init.Prescaler = 0;
  htim1.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim1.Init.Period = 65535;
  htim1.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim1.Init.RepetitionCounter = 0;
  htim1.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim1) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim1, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_IC_Init(&htim1) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim1, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigIC.ICPolarity = TIM_INPUTCHANNELPOLARITY_RISING;
  sConfigIC.ICSelection = TIM_ICSELECTION_DIRECTTI;
  sConfigIC.ICPrescaler = TIM_ICPSC_DIV1;
  sConfigIC.ICFilter = 0;
  if (HAL_TIM_IC_ConfigChannel(&htim1, &sConfigIC, TIM_CHANNEL_1) != HAL_OK)
  {
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
static void MX_TIM2_Init(void)
{

  /* USER CODE BEGIN TIM2_Init 0 */

  /* USER CODE END TIM2_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_IC_InitTypeDef sConfigIC = {0};

  /* USER CODE BEGIN TIM2_Init 1 */

  /* USER CODE END TIM2_Init 1 */
  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 0;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 4294967295;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_IC_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigIC.ICPolarity = TIM_INPUTCHANNELPOLARITY_RISING;
  sConfigIC.ICSelection = TIM_ICSELECTION_DIRECTTI;
  sConfigIC.ICPrescaler = TIM_ICPSC_DIV1;
  sConfigIC.ICFilter = 0;
  if (HAL_TIM_IC_ConfigChannel(&htim2, &sConfigIC, TIM_CHANNEL_1) != HAL_OK)
  {
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
static void MX_TIM3_Init(void)
{

  /* USER CODE BEGIN TIM3_Init 0 */

  /* USER CODE END TIM3_Init 0 */

  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_OC_InitTypeDef sConfigOC = {0};

  /* USER CODE BEGIN TIM3_Init 1 */

  /* USER CODE END TIM3_Init 1 */
  htim3.Instance = TIM3;
  htim3.Init.Prescaler = 0;
  htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim3.Init.Period = 65535;
  htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_PWM_Init(&htim3) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 0;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  if (HAL_TIM_PWM_ConfigChannel(&htim3, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
  {
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
static void MX_TIM4_Init(void)
{

  /* USER CODE BEGIN TIM4_Init 0 */

  /* USER CODE END TIM4_Init 0 */

  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_OC_InitTypeDef sConfigOC = {0};

  /* USER CODE BEGIN TIM4_Init 1 */

  /* USER CODE END TIM4_Init 1 */
  htim4.Instance = TIM4;
  htim4.Init.Prescaler = 1000;
  htim4.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim4.Init.Period = 100;
  htim4.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim4.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_PWM_Init(&htim4) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim4, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 0;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  if (HAL_TIM_PWM_ConfigChannel(&htim4, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_ConfigChannel(&htim4, &sConfigOC, TIM_CHANNEL_2) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_ConfigChannel(&htim4, &sConfigOC, TIM_CHANNEL_3) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_ConfigChannel(&htim4, &sConfigOC, TIM_CHANNEL_4) != HAL_OK)
  {
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
static void MX_TIM5_Init(void)
{

  /* USER CODE BEGIN TIM5_Init 0 */

  /* USER CODE END TIM5_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_OC_InitTypeDef sConfigOC = {0};

  /* USER CODE BEGIN TIM5_Init 1 */

  /* USER CODE END TIM5_Init 1 */
  htim5.Instance = TIM5;
  htim5.Init.Prescaler = 1000-1;
  htim5.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim5.Init.Period = 1000-1;
  htim5.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim5.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim5) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim5, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_Init(&htim5) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim5, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 0;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  if (HAL_TIM_PWM_ConfigChannel(&htim5, &sConfigOC, TIM_CHANNEL_3) != HAL_OK)
  {
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
static void MX_TIM9_Init(void)
{

  /* USER CODE BEGIN TIM9_Init 0 */

  /* USER CODE END TIM9_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};

  /* USER CODE BEGIN TIM9_Init 1 */

  /* USER CODE END TIM9_Init 1 */
  htim9.Instance = TIM9;
  htim9.Init.Prescaler = 50000-1;
  htim9.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim9.Init.Period = 100;
  htim9.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim9.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim9) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim9, &sClockSourceConfig) != HAL_OK)
  {
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
static void MX_TIM10_Init(void)
{

  /* USER CODE BEGIN TIM10_Init 0 */

  /* USER CODE END TIM10_Init 0 */

  TIM_OC_InitTypeDef sConfigOC = {0};

  /* USER CODE BEGIN TIM10_Init 1 */

  /* USER CODE END TIM10_Init 1 */
  htim10.Instance = TIM10;
  htim10.Init.Prescaler = 1000-1;
  htim10.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim10.Init.Period = 1000-1;
  htim10.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim10.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim10) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_Init(&htim10) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 0;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  if (HAL_TIM_PWM_ConfigChannel(&htim10, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
  {
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
static void MX_USART1_UART_Init(void)
{

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
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */

  /* USER CODE END USART1_Init 2 */

}

/**
  * Enable DMA controller clock
  */
static void MX_DMA_Init(void)
{

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
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOE, GPIO_PIN_3|GPIO_PIN_4|GPIO2_2_Pin|Laser_pointer_Pin
                          |GPIO3_2_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, BLDC_1_Pin|BLDC_2_Pin|Motor3_DIR1_Pin|LinearActuator_dir_Pin
                          |LinearActuator_EN_Pin|LinearActuator_DIR1_Pin|LinearActuator_DIR2_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(LinearActuator_en_GPIO_Port, LinearActuator_en_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOD, PassingMotor_DIR_Pin|LiftingMotor_DIR_Pin|LOCO_DIR1_Pin|LOCO_DIR2_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(STEPPER_DIR_GPIO_Port, STEPPER_DIR_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : Limit_switch_input_Pin */
  GPIO_InitStruct.Pin = Limit_switch_input_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(Limit_switch_input_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : PE3 PE4 GPIO2_2_Pin Laser_pointer_Pin
                           GPIO3_2_Pin */
  GPIO_InitStruct.Pin = GPIO_PIN_3|GPIO_PIN_4|GPIO2_2_Pin|Laser_pointer_Pin
                          |GPIO3_2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

  /*Configure GPIO pins : BLDC_1_Pin BLDC_2_Pin Motor3_DIR1_Pin LinearActuator_dir_Pin
                           LinearActuator_EN_Pin LinearActuator_DIR1_Pin LinearActuator_DIR2_Pin */
  GPIO_InitStruct.Pin = BLDC_1_Pin|BLDC_2_Pin|Motor3_DIR1_Pin|LinearActuator_dir_Pin
                          |LinearActuator_EN_Pin|LinearActuator_DIR1_Pin|LinearActuator_DIR2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pin : LinearActuator_en_Pin */
  GPIO_InitStruct.Pin = LinearActuator_en_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(LinearActuator_en_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : PassingMotor_DIR_Pin LiftingMotor_DIR_Pin LOCO_DIR1_Pin LOCO_DIR2_Pin */
  GPIO_InitStruct.Pin = PassingMotor_DIR_Pin|LiftingMotor_DIR_Pin|LOCO_DIR1_Pin|LOCO_DIR2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  /*Configure GPIO pin : STEPPER_DIR_Pin */
  GPIO_InitStruct.Pin = STEPPER_DIR_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(STEPPER_DIR_GPIO_Port, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	//test +=1;

	if(laserDebounce>0){
		laserDebounce--;
	}
	if(passDebounce>0){
		passDebounce--;
	}

	if(linearDebounce>0){
		linearDebounce--;

	}
	if(bldcDebounce>0){
		bldcDebounce--;
	}
	if(debounce>0){
				debounce --;
			}
	if(start_stepper){
		stepperCount+=1;
	}
	if(htim == &htim9){  //updates every 100ms
		flag = 1;

	}
	if(start_stepper){
		stepperIsOn += 1;
		if(stepperIsOn == 10){
			start_stepper = 0;
			stepperIsOn = 0;
		}
	}

}
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

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
