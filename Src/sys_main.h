/*
 * main.h
 *
 *  Created on: 15 ���. 2017 �.
 *      Author: koko
 */

#ifndef SYS_MAIN_H_
#define SYS_MAIN_H_

#include "cmsis_os.h"
extern osThreadId LedTaskHandle;
extern UART_HandleTypeDef huart1;
extern osThreadId lcd_TaskHandle;

extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart2;

#endif /* SYS_MAIN_H_ */
