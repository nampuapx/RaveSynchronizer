


#include "stm32f1xx_hal.h"
#include "cmsis_os.h"

#include "MIDI_lib\midi_lib.h"

#include "midi_gen_logic.h"



extern osThreadId LedTaskHandle;



void MIDI_recive_clock_pulse_handler(void){

	BaseType_t xHigherPriorityTaskWoken;

	put_MIDI_clock();

	MIDI_start_status++;
	MIDI_start_status %= MIDI_restart_value;

	if(!MIDI_start_status){

		put_MIDI_start();

		xHigherPriorityTaskWoken = pdFALSE;
		xTaskNotifyFromISR( LedTaskHandle,
								( 1UL << 1UL ),
								eSetBits,
								&xHigherPriorityTaskWoken );
		portYIELD_FROM_ISR( xHigherPriorityTaskWoken );
	}
	else if(!(MIDI_start_status % MIDI_CLOCK_PER_BEAT)){

		xHigherPriorityTaskWoken = pdFALSE;
		xTaskNotifyFromISR( LedTaskHandle,
								( 1UL << 0UL ),
								eSetBits,
								&xHigherPriorityTaskWoken );
		portYIELD_FROM_ISR( xHigherPriorityTaskWoken );
	}


}


void MIDI_recive_start_handler(void){
	BaseType_t xHigherPriorityTaskWoken;


	put_MIDI_start();
	xHigherPriorityTaskWoken = pdFALSE;
	xTaskNotifyFromISR( LedTaskHandle,
							( 1UL << 1UL ),
							eSetBits,
							&xHigherPriorityTaskWoken );
	portYIELD_FROM_ISR( xHigherPriorityTaskWoken );

	MIDI_resturt_counter = 0;
	MIDI_start_status = 0;

}











