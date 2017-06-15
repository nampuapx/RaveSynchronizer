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
void start_button_handle(extLine_HandleTypeDef *start_button_extLine_struct){


	if(extLine_get_new_state(start_button_extLine_struct)){
		if(start_button_extLine_struct->extLine_level_status == extLine_level_ZERO){
			//xTaskNotify(LedTaskHandle, ( 1UL << 1UL ), eSetBits );

			MIDI_resturt_counter = 0;
			MIDI_start_status = 0;
			put_MIDI_start();


			xTaskNotify( LedTaskHandle,
									( 1UL << 1UL ),
									eSetBits );


		}
	}

}

