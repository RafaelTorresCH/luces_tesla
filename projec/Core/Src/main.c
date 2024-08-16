/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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
UART_HandleTypeDef huart2;

/* USER CODE BEGIN PV */
uint32_t left_toggles = 0;
uint32_t righ_toggles = 0;
uint32_t park_toggles = 0;
uint32_t left_last_press_tick = 0;
uint32_t righ_last_press_tick = 0;
uint32_t park_last_press_tick = 0;
uint8_t status = 0;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART2_UART_Init(void);
/* USER CODE BEGIN PFP */
void heartbeat(void);
void turn_signal_left(void);
void turn_signal_righ(void);
void park_signal(void);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	if (GPIO_Pin == S1_Pin) {
		//s1: letf directional
		status = 1;
		HAL_UART_Transmit(&huart2, "S1\n", 3, 10);
		if (HAL_GetTick() < (left_last_press_tick + 300)) { // if last press was in the last 300ms
			left_toggles = 0xFFFFFF; // a long time toggling (infinite)
		} else {
			left_toggles = 6;
			righ_toggles = 0;
			park_toggles = 0;
		}
		left_last_press_tick = HAL_GetTick();
	}

	if (GPIO_Pin == S3_Pin) {
		//s3: righ directional
		status = 1;
		HAL_UART_Transmit(&huart2, "S3\n", 3, 10);
		if(HAL_GetTick() < righ_last_press_tick + 300 ){
			righ_toggles = 0xFFFFFF; // a long time toggling (infinite)
		}
		else{
			righ_toggles = 6;
			left_toggles = 0;
			park_toggles = 0;
		}
		righ_last_press_tick = HAL_GetTick();

	}

	if (GPIO_Pin == S2_Pin) {
			//s2: park directional
			if(status == 1){
				HAL_GPIO_WritePin(D3_GPIO_Port, D3_Pin, 1);
				HAL_GPIO_WritePin(D4_GPIO_Port, D4_Pin, 1);
				status = 0;
			}
			HAL_UART_Transmit(&huart2, "S2\r\n", 4, 10);
			if(HAL_GetTick() < park_last_press_tick + 300 ){
				park_toggles = 0xFFFFFF; // a long time toggling (infinite)
			}
			else{
				park_toggles = 6;
				left_toggles = 0;
				righ_toggles = 0;
			}
			park_last_press_tick = HAL_GetTick();

		}

}



void heartbeat(void)
{
	static uint32_t heartbeat_tick = 0;
	if(heartbeat_tick < HAL_GetTick()){
		heartbeat_tick = HAL_GetTick() + 1000;
		HAL_GPIO_TogglePin(D1_GPIO_Port, D1_Pin);
	}

}

//direccional izquierda

void turn_signal_left(void)
{
	static uint32_t turn_toggle_tick = 0;
	if (turn_toggle_tick < HAL_GetTick()) {
		if (left_toggles > 0) {
			turn_toggle_tick = HAL_GetTick() + 500;
			HAL_GPIO_TogglePin(D3_GPIO_Port, D3_Pin);
			left_toggles--;
		}else {
			HAL_GPIO_WritePin(D3_GPIO_Port, D3_Pin, 1);
		}

	}
}

void turn_signal_righ(void)
{
	static uint32_t turn_toggle_tick2 = 0;
	if (turn_toggle_tick2 < HAL_GetTick()) {
		if (righ_toggles > 0) {
			turn_toggle_tick2 = HAL_GetTick() + 500;
			HAL_GPIO_TogglePin(D4_GPIO_Port, D4_Pin);
			righ_toggles--;
		}else {
			HAL_GPIO_WritePin(D4_GPIO_Port, D4_Pin, 1);
		}

	}
}

void park_signal(void)
{

	static uint32_t turn_toggle_tick3 = 0;
	if (turn_toggle_tick3 < HAL_GetTick()) {
		if (park_toggles > 0) {
			turn_toggle_tick3 = HAL_GetTick() + 500;
			HAL_GPIO_TogglePin(D4_GPIO_Port, D4_Pin);
			HAL_GPIO_TogglePin(D3_GPIO_Port, D3_Pin);
			park_toggles--;
		}else{
			HAL_GPIO_WritePin(D4_GPIO_Port, D4_Pin, 1);
			HAL_GPIO_WritePin(D3_GPIO_Port, D3_Pin, 1);
		}

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
  MX_USART2_UART_Init();
  /* USER CODE BEGIN 2 */

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
	  heartbeat();
	  if(park_toggles != 0){
		  park_signal();
	  }else{
		  turn_signal_left();
		  turn_signal_righ();
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
  if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_MSI;
  RCC_OscInitStruct.MSIState = RCC_MSI_ON;
  RCC_OscInitStruct.MSICalibrationValue = 0;
  RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_6;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_MSI;
  RCC_OscInitStruct.PLL.PLLM = 1;
  RCC_OscInitStruct.PLL.PLLN = 40;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV7;
  RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
  RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
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

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 115200;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  huart2.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart2.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

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
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, D1_Pin|D3_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(D4_GPIO_Port, D4_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : PC13 */
  GPIO_InitStruct.Pin = GPIO_PIN_13;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pin : S1_Pin */
  GPIO_InitStruct.Pin = S1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(S1_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : S2_Pin */
  GPIO_InitStruct.Pin = S2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(S2_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : D1_Pin D3_Pin */
  GPIO_InitStruct.Pin = D1_Pin|D3_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : S3_Pin */
  GPIO_InitStruct.Pin = S3_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(S3_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : D4_Pin */
  GPIO_InitStruct.Pin = D4_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(D4_GPIO_Port, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI0_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI0_IRQn);

  HAL_NVIC_SetPriority(EXTI1_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI1_IRQn);

  HAL_NVIC_SetPriority(EXTI4_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI4_IRQn);

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
