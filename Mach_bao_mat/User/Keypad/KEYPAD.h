#ifndef __KEYPAD_H
#define __KEYPAD_H

#include "stm32f1xx.h"

#define COL1_PORT GPIOB
#define COL1_PIN GPIO_PIN_15

#define COL2_PORT GPIOB
#define COL2_PIN GPIO_PIN_13

#define COL3_PORT GPIOA
#define COL3_PIN GPIO_PIN_9

#define ROW1_PORT GPIOB
#define ROW1_PIN GPIO_PIN_14

#define ROW2_PORT GPIOA
#define ROW2_PIN GPIO_PIN_11

#define ROW3_PORT GPIOA
#define ROW3_PIN GPIO_PIN_10

#define ROW4_PORT GPIOA
#define ROW4_PIN GPIO_PIN_8

uint8_t Key_Scan();

#endif