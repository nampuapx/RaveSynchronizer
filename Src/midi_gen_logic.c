/*******************************************************************************
 *
 * midi_gen: is midi_gen
 *
 *    midi_gen:                      copyright 2017       Vetrov Nikolay.
 *
 *    Permission to use, copy, modify, and distribute this software in source
 *    and binary forms and its documentation for any purpose and without fee
 *    is hereby granted, provided that the above copyright notice appear
 *    in all copies and that both that copyright notice and this permission
 *    notice appear in supporting documentation.
 *
 *    THIS SOFTWARE IS PROVIDED BY THE VETROV NIKOLAY AND CONTRIBUTORS "AS IS"
 *    AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 *    IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 *    PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL VETROV NIKOLAY OR CONTRIBUTORS BE
 *    LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 *    CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 *    SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 *    INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 *    CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *    ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
 *    THE POSSIBILITY OF SUCH DAMAGE.
 *
 ******************************************************************************/
//#include "main.h"

#include "stm32f1xx_hal.h"
#include "cmsis_os.h"
#include "ext_line.h"
#include "encoder.h"
#include "MIDI_lib\midi_lib.h"
#include <sys_main.h>
#include "midi_gen_logic.h"
#include "gui.h"

#include "usbd_def.h"

extern uint8_t uartRX_byte;

extLine_HandleTypeDef

						enc01_extLine_struct,
						enc02_extLine_struct,
						start_request__button_extLine_struct,
						ExtInt_switch_extLine_struct;

encoder_HandleTypeDef	enc01_struct;

#define BUTTON_BLINK_DELAY 50

extern TIM_HandleTypeDef	htim1;
#define SEC_IN_MIN	60
#define MIDI_CLOCK_TIMER_PRESCALER	htim1.Init.Prescaler
#define MIDI_CLOCK_TIMER_PERIOD		htim1.Init.Period

#define TIMER_PRESCALER_VALUE	100 // don't change this!!!
#define DEF_BPM		100
#define BPM_MIN		30
#define BPM_MAX		300

uint16_t bpm =  DEF_BPM;



#define DEFAULT_RESTART_STEPS_VALUE	(MIDI_CLOCK_PER_BEAT * 4) //16 steps
uint8_t MIDI_start_status = 0;
uint16_t MIDI_resturt_counter,MIDI_restart_value = DEFAULT_RESTART_STEPS_VALUE;

uint8_t need_start = 0;

_ext_int_state ext_int_state = internal_clock_and_transport;








//////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////
void bpm_hardware_timer_setup(uint16_t bpm_val){

uint32_t	period_init_val = 0;

	period_init_val = HAL_RCC_GetHCLKFreq();
	period_init_val	/= TIMER_PRESCALER_VALUE;
	period_init_val	/= MIDI_CLOCK_PER_BEAT;
	period_init_val *= SEC_IN_MIN;

	period_init_val /= bpm_val;

	MIDI_CLOCK_TIMER_PRESCALER = TIMER_PRESCALER_VALUE-1;
	MIDI_CLOCK_TIMER_PERIOD = (uint16_t)period_init_val;



	if (HAL_TIM_Base_Init(&htim1) != HAL_OK)
	  {
	    _Error_Handler(__FILE__, __LINE__);
	  }
}

void encoder_stepup(encoder_HandleTypeDef * enc_struct){
	bpm++;
	if(bpm>BPM_MAX)bpm = BPM_MAX;
	bpm_hardware_timer_setup(bpm);
	gui_print_lcd_bpm();
}

void encoder_stepdown(encoder_HandleTypeDef * enc_struct){
	bpm--;
	if(bpm<BPM_MIN)bpm = BPM_MIN;
	bpm_hardware_timer_setup(bpm);
	gui_print_lcd_bpm();
}



extern USBD_HandleTypeDef hUsbDeviceFS;
void Perf_Task(void){
	uint8_t	led_trigger,led_trigger_val;
	uint16_t	note_dede = 0;

	extLine_init(&start_request__button_extLine_struct, bttn_start_request_GPIO_Port, bttn_start_request_Pin);
	extLine_init(&ExtInt_switch_extLine_struct, switch_ExtInt_GPIO_Port, switch_ExtInt_Pin);
	extLine_init(&enc01_extLine_struct, enc01_ch1_GPIO_Port, enc01_ch1_Pin);
	extLine_init(&enc02_extLine_struct, enc01_ch2_GPIO_Port, enc01_ch2_Pin);

	enc01_struct.line01 = &enc01_extLine_struct;
	enc01_struct.line02 = &enc02_extLine_struct;

	HAL_TIM_Base_Start_IT(&htim1);

    if (HAL_UART_Receive_IT(&huart1, (uint8_t *)&uartRX_byte, 1) != HAL_OK) {
        Error_Handler();
    }

    bpm_hardware_timer_setup(100);

    osDelay(2);
    gui_print_lcd_bpm();

	  for(;;)
	  {
		  encoder_handle(&enc01_struct);

		  start_request_button_handle(&start_request__button_extLine_struct);

		  ExtInt_switch_handle(&ExtInt_switch_extLine_struct);
		//  need_start = 5;
		  if(need_start){
			  if(led_trigger){
				  if(!led_trigger_val--){
					  led_trigger_val = BUTTON_BLINK_DELAY;
					  led_trigger = 0;
					  Onboard_led_ON();
					 // lamp01_ON();
				  }
			  }else{
				  if(!led_trigger_val--){
					  led_trigger_val = BUTTON_BLINK_DELAY;
					  led_trigger = 5;
					  Onboard_led_OFF();
					 // lamp01_OFF();
				  }
			  }
		  }//if(need_start){

//		  note_dede++;
//		  note_dede%=500;
//		  if(!note_dede){
//
//			  Onboard_led_TOGG();
//			  if(hUsbDeviceFS.dev_state == USBD_STATE_CONFIGURED){
//				  led_01_TOGG();
//				  sendNoteOn(2, 20, 55);
//				  USBD_MIDI_SendPacket();
//			  }
//		  }

		  osDelay(1);

	  }
}


void TIM1_PeriodElapsedCallback(void){

//	BaseType_t xHigherPriorityTaskWoken;
//
//
//	xHigherPriorityTaskWoken = pdFALSE;
//	xTaskNotifyFromISR( lamp_TaskHandle,
//							( 1UL << 0UL ),
//							eSetBits,
//							&xHigherPriorityTaskWoken );
//	portYIELD_FROM_ISR( xHigherPriorityTaskWoken );


	if(ext_int_state == internal_clock_and_transport){
		clock_pulse_event_handler();
	}
}







void led_task(void){

	uint32_t ulNotifiedValue;

	for(;;)
	  {
			xTaskNotifyWait( 	0x00,      /* Don't clear any notification bits on entry. */
								0xffffffff , /* Reset the notification value to 0 on exit. */
								&ulNotifiedValue, /* Notified value pass out in
												  ulNotifiedValue. */
								portMAX_DELAY );  /* Block indefinitely. */

			if(( ulNotifiedValue & 0x02 ) != 0 ){
				Onboard_led_ON();
				osDelay(200);
				Onboard_led_OFF();
			}

	        if(( ulNotifiedValue & 0x01 ) != 0 )
	        {
	        	Onboard_led_ON();
				osDelay(10);
				Onboard_led_OFF();
	        }
	  }
}













