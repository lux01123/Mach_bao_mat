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
#include "stdio.h"
#include "string.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
typedef struct
{
	uint8_t sec;
	uint8_t min;
	uint8_t hour;
	uint8_t day;
	uint8_t date;
	uint8_t month;
	uint8_t year;
}
DS1307_TIME;
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define PASSWORD   0x0800FC00     // ngan nho 64
#define TIMECHECK  0x0800F800     // ngan nho 63
#define PASSWORDCHECK        0x0800F400     // ngan nho 62
//#define          0x0800F000
#define DS1307_ADDR (0x68 << 1)
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
I2C_HandleTypeDef hi2c1;

/* USER CODE BEGIN PV */
DS1307_TIME time = 
{
	.sec = 30,
	.min = 25,
	.hour = 10,
	.day = 5,
	.date = 17,
	.month = 2,
	.year = 22,
};

uint8_t i;

int8_t chectime;    // kiem tra xem da luu thoi gian choi
int16_t mk;          // gia tri trung gian kiem tra mat khau trong ngan nho
int64_t testtime;   // bien kiem tra hieu thoi gian gio phut giay
int64_t testday;    // bien kiem tra hieu thoi gian ngay thang

int8_t key;
char szKey[20];

int8_t kt = 0;

uint8_t timelcd[25];     // mang thoi gian de hien thi lcd
uint8_t datelcd[25];		 // mang ngay thang de hien thi lcd

/*** Cac bien cho mk ***/


/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_I2C1_Init(void);

void DangNhap (void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
uint8_t B2D(uint8_t num)
{
	return ((num >> 4)*10 + (num & 0x0F));
}

uint8_t D2B(uint8_t num)
{
	return ((num/10)<<4 | (num%10));
}

void SET_TIME(DS1307_TIME *setTime)
{
	uint8_t tempTime[7];
	tempTime[0] = D2B(setTime -> sec);
	tempTime[1] = D2B(setTime -> min);
	tempTime[2] = D2B(setTime -> hour);
	tempTime[3] = D2B(setTime -> day);
	tempTime[4] = D2B(setTime -> date);
	tempTime[5] = D2B(setTime -> month);
	tempTime[6] = D2B(setTime -> year);
	
	HAL_I2C_Mem_Write(&hi2c1, DS1307_ADDR, 0x00, 1, tempTime, sizeof(tempTime), 1000);
}

void GET_TIME(DS1307_TIME *getTime)
{
	uint8_t tempTime[7];
	HAL_I2C_Mem_Read(&hi2c1, DS1307_ADDR, 0x00, 1, tempTime, sizeof(tempTime), 1000);
	getTime -> sec   = B2D(tempTime[0]);
	getTime -> min   = B2D(tempTime[1]);
	getTime -> hour  = B2D(tempTime[2]);
	getTime -> day   = B2D(tempTime[3]);
	getTime -> date  = B2D(tempTime[4]);
	getTime -> month = B2D(tempTime[5]);
	getTime -> year  = B2D(tempTime[6]);
}
	
uint8_t sec;
uint8_t min;
uint8_t hour;
uint8_t day;        // ngay trong tuan
uint8_t date;			  // ngay trong thang
uint8_t month;
uint8_t year;

int64_t a = 0;

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
  MX_I2C1_Init();
  /* USER CODE BEGIN 2 */
	//SET_TIME(&time);  // cai thoi gian lan dau
	
/****** Chuong trinh su dung Flash de kiem tra thoi gian ************/	
//	chectime = Flash_Read_Int(TIMECHECK);
//	if( chectime != 2022)
//	{
//		SET_TIME(&time);
//		Flash_Erase(TIMECHECK);
//		Flash_Write_Int(TIMECHECK, 2022);
//	}
//	else if (chectime == 2022)
//	{
//		GET_TIME(&time);
//	}
/************************************************************************/

	lcd_init();
	lcd_clear();
/**********************************************************************/
// 	Flash_Erase(PASSWORDCHECK);                                // xoa o nho de test chuong trinh hoac khi khoi dong lai
	mk = Flash_Read_Int(PASSWORDCHECK);
	if (mk == 2022)
	{ 
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_11, GPIO_PIN_SET);
		GET_TIME(&time);
	}
	else if (mk != 2022)
	{
		GET_TIME(&time);
		testtime = (time.sec - 0) + (time.min - 30)*64 + (time.hour - 15)*4000;    // chuong trinh test thoi gian ngan
		testday = (time.date - 1) + (time.month - 6)*32 + (time.year - 22)*444;    // cai dat ngay khoa relay
		if ((time.date >= 1) && (time.month >= 1) && (time.year >= 22))            // chuong trinh phu dam bao thoi gian doc duoc tu ds1307
		{
			if (testday < 0)
			{
//				if (testtime < 0)
//				{
//					HAL_GPIO_WritePin(GPIOB, GPIO_PIN_11, GPIO_PIN_SET);
//				}
//				else if (testtime >= 0)
//				{
//					HAL_GPIO_WritePin(GPIOB, GPIO_PIN_11, GPIO_PIN_RESET);       // chuong trinh kiem tra thoi gian khi test
//				}
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_11, GPIO_PIN_SET);
			}
			if (testday >= 0)
			{
				HAL_GPIO_WritePin(GPIOB, GPIO_PIN_11, GPIO_PIN_RESET);
			}
		}
	}
/*********************************************************************/
	

/*************************************/

/*********************************************************************/
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
//		lcd_clear();
//    Check lan 1 thoi gian va hien thi LCD
//		GET_TIME(&time);
//		sprintf((char*)timelcd, "%02d:%02d:%02d", time.hour, time.min, time.sec);
//		sprintf((char*)datelcd, "%02d/%02d/20%02d", time.date, time.month, time.year);
//		lcd_puts(0, 0, (int8_t*)datelcd);
//		lcd_puts(1, 0, (int8_t*)timelcd);
		
//		HAL_Delay(1000);
//		i++;
		
// Check lan 2 kiem tra keypad
//      key = Key_Scan();		
//		  if (key != 0)
//			{
//				lcd_clear();
//				sprintf(szKey, "%d", key);
//				lcd_puts(0, 0, (int8_t*)szKey);
//				i = i + key;
//			}

// Check lan 3 kiem tra mat khau
	GET_TIME(&time);
	sprintf((char*)timelcd, "%02d:%02d:%02d", time.hour, time.min, time.sec);
	sprintf((char*)datelcd, "%02d/%02d/20%02d", time.date, time.month, time.year);
	lcd_puts(0, 0, (int8_t*)datelcd);
	lcd_puts(1, 0, (int8_t*)timelcd);
	key = Key_Scan();
	if (key == 10)
	{
		lcd_clear();
		a= 0;
		lcd_puts(0, 0, (int8_t*)"Nhap mat khau");
		while (key != 12)
		{
			i=20;
			key = Key_Scan();			
			if ((key != 11) && (key !=10) && (key!=12))
			{
				i--;
				a = a*10 + key;
				sprintf(szKey, "%d", a);
				lcd_puts(1, 5, (int8_t*)szKey);
				HAL_Delay(700);

			}
		}
		if (key == 12)
		{
			if (a == 99872022)
			{
				lcd_clear();
				lcd_puts(1, 0, (int8_t*)"Mat khau dung");
				HAL_Delay(1500);
				a = 0;
				lcd_clear();
				HAL_GPIO_WritePin(GPIOB, GPIO_PIN_11, GPIO_PIN_SET);
				/************************/ 
// Chuong trinh de cai mat khau vinh vien 
				
		Flash_Erase(PASSWORDCHECK);
		Flash_Write_Int(PASSWORDCHECK, 2022);
			
				/************************/
			}
			else if (a != 99872022)
			{
				lcd_clear();
				lcd_puts(1, 0, (int8_t*)"Mat khau sai");
				HAL_Delay(1500);
				a = 0;
				lcd_clear();
				HAL_GPIO_WritePin(GPIOB, GPIO_PIN_11, GPIO_PIN_RESET);
				
// code bo sung ngay 18/2/2022
 			Flash_Erase(PASSWORDCHECK);
			}
		}
	}

/*******************************/
		//HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_11);
		//HAL_Delay(1000);
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

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSE;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief I2C1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C1_Init(void)
{

  /* USER CODE BEGIN I2C1_Init 0 */

  /* USER CODE END I2C1_Init 0 */

  /* USER CODE BEGIN I2C1_Init 1 */

  /* USER CODE END I2C1_Init 1 */
  hi2c1.Instance = I2C1;
  hi2c1.Init.ClockSpeed = 100000;
  hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C1_Init 2 */

  /* USER CODE END I2C1_Init 2 */

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
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_11|GPIO_PIN_13|GPIO_PIN_15|GPIO_PIN_3
                          |GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_8|GPIO_PIN_9, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9|GPIO_PIN_12|GPIO_PIN_15, GPIO_PIN_RESET);

  /*Configure GPIO pin : PC13 */
  GPIO_InitStruct.Pin = GPIO_PIN_13;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : PB11 PB13 PB15 PB3
                           PB4 PB5 PB8 PB9 */
  GPIO_InitStruct.Pin = GPIO_PIN_11|GPIO_PIN_13|GPIO_PIN_15|GPIO_PIN_3
                          |GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_8|GPIO_PIN_9;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pin : PB14 */
  GPIO_InitStruct.Pin = GPIO_PIN_14;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pins : PA8 PA10 PA11 */
  GPIO_InitStruct.Pin = GPIO_PIN_8|GPIO_PIN_10|GPIO_PIN_11;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : PA9 PA12 PA15 */
  GPIO_InitStruct.Pin = GPIO_PIN_9|GPIO_PIN_12|GPIO_PIN_15;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI15_10_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);

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
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
