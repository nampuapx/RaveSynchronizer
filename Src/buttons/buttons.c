/*
 * buttons.c
 *
 *  Created on: 15 θών. 2017 γ.
 *      Author: koko
 */

#include "stm32f1xx_hal.h"
#include "ext_line.h"
#include "MIDI_lib\midi_lib.h"
#include "midi_gen_logic.h"
#include <sys_main.h>



void start_request_button_handle(extLine_HandleTypeDef *start_button_extLine_struct){
	if(extLine_get_new_state(start_button_extLine_struct)){
		if(start_button_extLine_struct->extLine_level_status == extLine_level_ZERO){
			if(need_start){
				need_start = 0;
				Onboard_led_OFF();
				gui_print_start_wait_clear();
			}else{
				need_start = 5;
				gui_print_start_wait();
			}
		}
	}
}



void global_start_button_handle(extLine_HandleTypeDef *start_button_extLine_struct){

	if(ext_int_state == internal_clock_and_transport){
		if(extLine_get_new_state(start_button_extLine_struct)){
			if(start_button_extLine_struct->extLine_level_status == extLine_level_ZERO){
				start_event_handler();
			}
		}
	}
}


ExtInt_switch_handle(extLine_HandleTypeDef *start_button_extLine_struct){

	if(extLine_get_new_state(start_button_extLine_struct)){
		if(start_button_extLine_struct->extLine_level_status == extLine_level_ZERO){
			ext_int_state = external_clock_and_transport;
			gui_print_lcd_bpm();
		}else{
			ext_int_state = internal_clock_and_transport;
			gui_print_lcd_bpm();
		}
	}
}


