#include "KEYPAD.h"

uint8_t x = 30;
uint8_t Key_Scan(void)
{
	HAL_GPIO_WritePin(COL1_PORT, COL1_PIN, GPIO_PIN_SET);
	HAL_GPIO_WritePin(COL2_PORT, COL2_PIN, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(COL3_PORT, COL3_PIN, GPIO_PIN_RESET);
	
	if (HAL_GPIO_ReadPin(ROW1_PORT, ROW1_PIN) == 1)
	{	
		HAL_Delay(x);
		if (HAL_GPIO_ReadPin(ROW1_PORT, ROW1_PIN) == 1 )
		{
		while(HAL_GPIO_ReadPin(ROW1_PORT, ROW1_PIN) == 1 );
		return 1;
		}
	}

	if (HAL_GPIO_ReadPin(ROW2_PORT, ROW2_PIN) == 1)
	{	
		HAL_Delay(x);
		if (HAL_GPIO_ReadPin(ROW2_PORT, ROW2_PIN) == 1)
		{
		return 4;
		}
	}
	
	if (HAL_GPIO_ReadPin(ROW3_PORT, ROW3_PIN) == 1)
	{	
		HAL_Delay(x);
		if (HAL_GPIO_ReadPin(ROW3_PORT, ROW3_PIN) == 1)
		{
		return 7;
		}
	}
	if (HAL_GPIO_ReadPin(ROW4_PORT, ROW4_PIN) == 1)
	{	
		HAL_Delay(x);
		if (HAL_GPIO_ReadPin(ROW4_PORT, ROW4_PIN) == 1)
		{
		return 10;
		}
	}
	
	HAL_GPIO_WritePin(COL1_PORT, COL1_PIN, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(COL2_PORT, COL2_PIN, GPIO_PIN_SET);
	HAL_GPIO_WritePin(COL3_PORT, COL3_PIN, GPIO_PIN_RESET);
	if (HAL_GPIO_ReadPin(ROW1_PORT, ROW1_PIN) == 1)
	{	
		HAL_Delay(x);
		if (HAL_GPIO_ReadPin(ROW1_PORT, ROW1_PIN) == 1)
		{
		return 2;
		}
	}
	if (HAL_GPIO_ReadPin(ROW2_PORT, ROW2_PIN) == 1)
	{	
		HAL_Delay(x);
		if (HAL_GPIO_ReadPin(ROW2_PORT, ROW2_PIN) == 1)
		{
		return 5;
		}
	}
	if (HAL_GPIO_ReadPin(ROW3_PORT, ROW3_PIN) == 1 )
	{	
		HAL_Delay(x);
		if (HAL_GPIO_ReadPin(ROW3_PORT, ROW3_PIN) == 1)
		{
		return 8;
		}
	}
	if (HAL_GPIO_ReadPin(ROW4_PORT, ROW4_PIN) == 1)
	{	
		HAL_Delay(x);
		if (HAL_GPIO_ReadPin(ROW4_PORT, ROW4_PIN) == 1)
		{
		return 0;
		}
	}
	
	HAL_GPIO_WritePin(COL1_PORT, COL1_PIN, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(COL2_PORT, COL2_PIN, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(COL3_PORT, COL3_PIN, GPIO_PIN_SET);
	
	if (HAL_GPIO_ReadPin(ROW1_PORT, ROW1_PIN) == 1)
	{	
		HAL_Delay(x);
		if (HAL_GPIO_ReadPin(ROW1_PORT, ROW1_PIN) == 1)
		{
		return 3;
		}
	}
	if (HAL_GPIO_ReadPin(ROW2_PORT, ROW2_PIN) == 1)
	{	
		HAL_Delay(x);
		if (HAL_GPIO_ReadPin(ROW2_PORT, ROW2_PIN) == 1)
		{
		return 6;
		}
	}
	if (HAL_GPIO_ReadPin(ROW3_PORT, ROW3_PIN) == 1)
	{	
		HAL_Delay(x);
		if (HAL_GPIO_ReadPin(ROW3_PORT, ROW3_PIN) == 1)
		{
		return 9;
		}
	}
	if (HAL_GPIO_ReadPin(ROW4_PORT, ROW4_PIN) == 1)
	{	
		HAL_Delay(x);
		if (HAL_GPIO_ReadPin(ROW4_PORT, ROW4_PIN) == 1)
		{
		return 12;
		}
	}
  return 11; 
}
	