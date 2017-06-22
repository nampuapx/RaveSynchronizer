/*
 * midi_gen_events.c
 *
 *  Created on: 15 θών. 2017 γ.
 *      Author: koko
 */
#include "stm32f1xx_hal.h"
#include "cmsis_os.h"
#include "MIDI_lib\midi_lib.h"
#include "midi_gen_logic.h"

extern osThreadId LedTaskHandle;

void clock_pulse_event_handler(void){

	BaseType_t xHigherPriorityTaskWoken;


	UART_2_plain_MIDI_select();
	put_MIDI_clock();
	UART_2_MAX485_switch();
	put_MIDI_clock();

	USBMIDIsend_MIDIClock(0);
	USBMIDIsend_MIDIClock(1);
	USBD_MIDI_SendPacket();

	MIDI_start_status++;
	MIDI_start_status %= MIDI_restart_value;

	if(!MIDI_start_status){

		put_MIDI_start();

		if(need_start){
			UART_2_plain_MIDI_select();
			put_MIDI_start();

			need_start = 0;
			gui_print_start_wait_clear();
		}

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

	gui_print_lcd_step();

}





void start_event_handler(void){

	BaseType_t xHigherPriorityTaskWoken;


	UART_2_plain_MIDI_select();
	put_MIDI_start();
	UART_2_MAX485_switch();
	put_MIDI_start();

	need_start = 0;

	xHigherPriorityTaskWoken = pdFALSE;
	xTaskNotifyFromISR( LedTaskHandle,
							( 1UL << 1UL ),
							eSetBits,
							&xHigherPriorityTaskWoken );
	portYIELD_FROM_ISR( xHigherPriorityTaskWoken );

	MIDI_resturt_counter = 0;
	MIDI_start_status = 0;

}



