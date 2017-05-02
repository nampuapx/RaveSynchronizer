/*
 * ext_line.c
 *
 *  Created on: 2 мая 2017 г.
 *      Author: koko
 */

#include "stm32f1xx_hal.h"
#include "ext_line.h"



#define ANTI_DREBEZG_VALUE  5



void extLine_init(	extLine_HandleTypeDef * extLine_Handle_struct,
					GPIO_TypeDef* GPIOx,
					uint16_t GPIO_Pin){

	extLine_Handle_struct->line_state_counter = 0;
	extLine_Handle_struct->extLine_states = extLine_processing_UNDONE;
	extLine_Handle_struct->GPIOx = GPIOx;
	extLine_Handle_struct->GPIO_Pin = GPIO_Pin;
	extLine_Handle_struct->extLine_level_states = HAL_GPIO_ReadPin(GPIOx, GPIO_Pin);

}




uint8_t extLine_get_new_state(	extLine_HandleTypeDef * extLine_Handle_struct){

	if(HAL_GPIO_ReadPin(extLine_Handle_struct->GPIOx, extLine_Handle_struct->GPIO_Pin)){
		if(extLine_Handle_struct->extLine_level_states){
			return (0);
		}else{
			extLine_Handle_struct->line_state_counter++;
			if(extLine_Handle_struct->line_state_counter > ANTI_DREBEZG_VALUE){
				extLine_Handle_struct->line_state_counter = ANTI_DREBEZG_VALUE;
				extLine_Handle_struct->extLine_level_states = extLine_level_FULL;
				return (5);
			}
		}
	}else{

		if(extLine_Handle_struct->extLine_level_states){

			extLine_Handle_struct->line_state_counter--;
			if(extLine_Handle_struct->line_state_counter < -ANTI_DREBEZG_VALUE){
				extLine_Handle_struct->line_state_counter = -ANTI_DREBEZG_VALUE;
				extLine_Handle_struct->extLine_level_states = extLine_level_ZERO;
				return (5);
			}
		}else{
				return (0);
		}
	}
}



