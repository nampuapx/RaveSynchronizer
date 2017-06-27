/*
 * gui.c
 *
 *  Created on: 16 θών. 2017 γ.
 *      Author: koko
 */
#include "stm32f1xx_hal.h"
#include "cmsis_os.h"
#include <sys_main.h>
#include "midi_gen_logic.h"

#include "gui.h"
#include "LiquidCrystal_I2C.h"

xQueueHandle q_lcd = 0;



extern uint16_t bpm;
void gui_print_lcd_bpm(void){
#ifdef GUI_DISABLE
	return;
#endif
	type_q_lcd_element working_msg;

    working_msg.xy = 0x00;
    if(ext_int_state == internal_clock_and_transport){
    	sprintf(working_msg.txt,"BPM %3u",bpm);
    }else{
    	sprintf(working_msg.txt,"BPM EXT");
    }
    xQueueSend( q_lcd, (void *) &working_msg, portMAX_DELAY );
}


void gui_print_start_wait(void){
#ifdef GUI_DISABLE
	return;
#endif
	type_q_lcd_element working_msg;

    working_msg.xy = 0x90;
    sprintf(working_msg.txt,"WAIT");
    xQueueSend( q_lcd, (void *) &working_msg, portMAX_DELAY );
}

void gui_print_start_wait_clear(void){
#ifdef GUI_DISABLE
	return;
#endif
	type_q_lcd_element working_msg;
	BaseType_t xHigherPriorityTaskWoken;

    working_msg.xy = 0x90;
    sprintf(working_msg.txt,"    ");
    //xQueueSend( q_lcd, (void *) &working_msg, portMAX_DELAY );
    xQueueSendFromISR( q_lcd, (void *) &working_msg, &xHigherPriorityTaskWoken);
}




void gui_print_lcd_step(void){
#ifdef GUI_DISABLE
	return;
#endif
	type_q_lcd_element working_msg;
	BaseType_t xHigherPriorityTaskWoken;

	static uint8_t step_pos,pred_step_pos;


	if(q_lcd){
		step_pos = (uint8_t)(MIDI_start_status/6);
		if(step_pos != pred_step_pos){
			pred_step_pos = step_pos;

			working_msg.xy = 0x01;
			sprintf(working_msg.txt,"oooooooooooooooo");

			working_msg.txt[step_pos] = 'x';

			xQueueSendFromISR( q_lcd, (void *) &working_msg, &xHigherPriorityTaskWoken);

		}


	if(!(step_pos%4)){
					working_msg.xy = (0xf0);
					sprintf(working_msg.txt,"%c",(step_pos>>2)+1);

					xQueueSendFromISR( q_lcd, (void *) &working_msg, &xHigherPriorityTaskWoken);

	}

}

//	if(q_lcd){
//		step_pos = (uint8_t)(MIDI_start_status/12);
//		if(step_pos != pred_step_pos){
//			pred_step_pos = step_pos;
//
//			working_msg.xy = 0x01;
//			sprintf(working_msg.txt,"OOOOOOOOOOOOOOOO");
//
//
//			working_msg.txt[step_pos<<1] = '>';
//			working_msg.txt[(step_pos<<1)+1] = '<';
//
//
//			xQueueSendFromISR( q_lcd, (void *) &working_msg, &xHigherPriorityTaskWoken);
//
//		}
//	}

//
//	if(q_lcd){
//		step_pos = (uint8_t)(MIDI_start_status/12);
//		if(step_pos != pred_step_pos){
//			pred_step_pos = step_pos;
//
//			working_msg.xy = 0x01;
//			sprintf(working_msg.txt,"                ");
//			//sprintf(working_msg.txt,"<><><><><><><><>");
//			//sprintf(&(working_msg.txt[step_pos<<1]),"%c%c",4,5);
//
//			if(step_pos%2){
//				working_msg.txt[step_pos<<1] = '<';
//				working_msg.txt[(step_pos<<1)+1] = '>';
//			}else{
//				working_msg.txt[step_pos<<1] = 4;
//				working_msg.txt[(step_pos<<1)+1] = 5;
//			}
//
//			xQueueSendFromISR( q_lcd, (void *) &working_msg, &xHigherPriorityTaskWoken);
//
//
////			working_msg.xy = (uint8_t)(((step_pos)<<5)|0x01);
////			if(!step_pos){
////				sprintf(working_msg.txt,"%c%c",4,5);
////			}else{
////				sprintf(working_msg.txt,"<>");
////			}
////			xQueueSendFromISR( q_lcd, (void *) &working_msg, &xHigherPriorityTaskWoken);
//		}
//	}

}








//
//void displayKeyCodes(void) {
//  uint8_t i = 0;
//  while (1) {
//    LCDI2C_clear();
//    //LCDI2C_setCursor(2,2);
//    //LCDI2C_write_String("TEN Electronics");
//    LCDI2C_setCursor(0, 0);
//	char buf[10];
//	itoa(i, buf, 10);
//    LCDI2C_write_String("Cds 0x"); LCDI2C_write_String(buf);
//	itoa(i+19, buf, 10);
//    LCDI2C_write_String("-0x"); LCDI2C_write_String(buf);
//    LCDI2C_setCursor(0, 1);
//    int j;
//    for (j=0; j<20; j++) {
//      LCDI2C_write(i+j);
//    }
//    i+=16;
//    if (i<15) break;
//    osDelay(700);
//  }
//}



//const uint8_t bell[8]  = {0x4,0xe,0xe,0xe,0x1f,0x0,0x4};
//const uint8_t note[8]  = {0x2,0x3,0x2,0xe,0x1e,0xc,0x0};
//const uint8_t clock[8] = {0x0,0xe,0x15,0x17,0x11,0xe,0x0};
//const uint8_t heart[8] = {0x0,0xa,0x1f,0x1f,0xe,0x4,0x0};

const uint8_t b_01[8]  = {0x18,0x18,0x18,0x0,0x0,0x0,0x0};
const uint8_t b_02[8]  = {0x3,0x3,0x3,0x0,0x0,0x0,0x0};
const uint8_t b_03[8]  = {0x0,0x0,0x0,0x0,0x3,0x3,0x3};
const uint8_t b_04[8]  = {0x0,0x0,0x0,0x0,0x18,0x18,0x18};



const uint8_t ar_left[8]  = {0x1,0x3,0x7,0xf,0x7,0x3,0x1};
const uint8_t ar_right[8] = {0x8,0xc,0xe,0xf,0xe,0xc,0x8};
const uint8_t cross[8] = {0x0,0x1b,0xe,0x4,0xe,0x1b,0x0};
const uint8_t retarrow[8] = {0x1,0x1,0x5,0x9,0x1f,0x8,0x4};

void HAL_I2C_MasterTxCpltCallback(I2C_HandleTypeDef *hi2c)
{
	BaseType_t xHigherPriorityTaskWoken;

	xHigherPriorityTaskWoken = pdFALSE;
	xTaskNotifyFromISR( lcd_TaskHandle,
							( 1UL << 0UL ),
							eSetBits,
							&xHigherPriorityTaskWoken );
	portYIELD_FROM_ISR( xHigherPriorityTaskWoken );


}

void lcd_Task(void){

	uint32_t ulNotifiedValue;
	type_q_lcd_element working_msg;
	q_lcd = xQueueCreate( 8, sizeof( type_q_lcd_element ) );

	LCDI2C_init(0x4e,SIMBOL_LCD_WIDTH,SIMBOL_LCD_ROWS);
	LCDI2C_backlight();
	LCDI2C_createChar(0, (uint8_t*)retarrow);
	LCDI2C_createChar(1, (uint8_t*)b_01);
	LCDI2C_createChar(2, (uint8_t*)b_02);
	LCDI2C_createChar(3, (uint8_t*)b_03);
	LCDI2C_createChar(4, (uint8_t*)b_04);
	LCDI2C_createChar(5, (uint8_t*)ar_left);
	LCDI2C_createChar(6, (uint8_t*)ar_right);
	LCDI2C_createChar(7, (uint8_t*)cross);
	LCDI2C_createChar(8, (uint8_t*)retarrow);
	LCDI2C_clear();
	  //LCDI2C_setCursor(0,0);
	  //LCDI2C_write(4);

	//  Usart1_Send_String("End");


	for(;;){

		xQueueReceive( q_lcd, &( working_msg ), portMAX_DELAY );
		LCDI2C_setCursor(working_msg.xy>>4,working_msg.xy&0x0f);
		LCDI2C_write_String(working_msg.txt);
		//osDelay(2);

	}


}
