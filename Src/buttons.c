/*
 * buttons.c
 *
 *  Created on: 15 ���. 2017 �.
 *      Author: koko
 */

#include "stm32f1xx_hal.h"
#include "ext_line.h"
#include "MIDI_lib\midi_lib.h"
#include "midi_gen_logic.h"
#include <sys_main.h>


uint8_t ExtInt_predstate = 0;

void start_request_button_handle(extLine_HandleTypeDef *start_button_extLine_struct){
	if(extLine_get_new_state(start_button_extLine_struct)){
		if(start_button_extLine_struct->extLine_level_status == extLine_level_ZERO){
			need_start = 5;
		}
	}

}



void global_start_button_handle(extLine_HandleTypeDef *start_button_extLine_struct){

	if(ext_int_state == internal_clock_and_transport){
			if(extLine_get_new_state(start_button_extLine_struct)){
			if(start_button_extLine_struct->extLine_level_status == extLine_level_ZERO){
				//xTaskNotify(LedTaskHandle, ( 1UL << 1UL ), eSetBits );
				start_event_handler();
	//			MIDI_resturt_counter = 0;
	//			MIDI_start_status = 0;
	//			put_MIDI_start();
	//
	//
	//			xTaskNotify( LedTaskHandle,
	//									( 1UL << 1UL ),
	//									eSetBits );
			}
		}
	}
}


ExtInt_switch_handle(extLine_HandleTypeDef *start_button_extLine_struct){

	if(extLine_get_new_state(start_button_extLine_struct)){
		if(start_button_extLine_struct->extLine_level_status == extLine_level_ZERO){
			ext_int_state = external_clock_and_transport;
		}else{
			ext_int_state = internal_clock_and_transport;
		}
	}

}


