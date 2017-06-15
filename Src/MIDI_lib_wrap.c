

#include "stm32f1xx_hal.h"
#include "midi_gen_events.h"
#include "midi_gen_logic.h"


void MIDI_recive_clock_pulse_handler(void){

	if(ext_int_state == external_clock_and_transport){
		clock_pulse_event_handler();
	}

}


void MIDI_recive_start_handler(void){

	if(ext_int_state == external_clock_and_transport){
		start_event_handler();
	}

}











