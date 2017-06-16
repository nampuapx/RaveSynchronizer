/*
 * gui.c
 *
 *  Created on: 16 θών. 2017 γ.
 *      Author: koko
 */
#include "stm32f1xx_hal.h"
#include "cmsis_os.h"
#include <sys_main.h>

#include "gui.h"

xQueueHandle q_lcd;




extern uint16_t bpm;
void gui_print_lcd_bpm(void){
	type_q_lcd_element working_msg;

    working_msg.xy = 0x50;
    sprintf(working_msg.txt,"BPM%3u",bpm);
    xQueueSend( q_lcd, ( void * ) &working_msg, portMAX_DELAY );

}






























void displayKeyCodes(void) {
  uint8_t i = 0;
  while (1) {
    LCDI2C_clear();
    //LCDI2C_setCursor(2,2);
    //LCDI2C_write_String("TEN Electronics");
    LCDI2C_setCursor(0, 0);
	char buf[10];
	itoa(i, buf, 10);
    LCDI2C_write_String("Cds 0x"); LCDI2C_write_String(buf);
	itoa(i+19, buf, 10);
    LCDI2C_write_String("-0x"); LCDI2C_write_String(buf);
    LCDI2C_setCursor(0, 1);
    int j;
    for (j=0; j<20; j++) {
      LCDI2C_write(i+j);
    }
    i+=16;
    if (i<15) break;
    osDelay(700);
  }
}



const uint8_t bell[8]  = {0x4,0xe,0xe,0xe,0x1f,0x0,0x4};
const uint8_t note[8]  = {0x2,0x3,0x2,0xe,0x1e,0xc,0x0};
const uint8_t clock[8] = {0x0,0xe,0x15,0x17,0x11,0xe,0x0};
const uint8_t heart[8] = {0x0,0xa,0x1f,0x1f,0xe,0x4,0x0};
const uint8_t duck[8]  = {0x0,0xc,0x1d,0xf,0xf,0x6,0x0};
const uint8_t check[8] = {0x0,0x1,0x3,0x16,0x1c,0x8,0x0};
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
	LCDI2C_createChar(0, bell);
	LCDI2C_createChar(1, note);
	LCDI2C_createChar(2, clock);
	LCDI2C_createChar(3, heart);
	LCDI2C_createChar(4, duck);
	LCDI2C_createChar(5, check);
	LCDI2C_createChar(6, cross);
	LCDI2C_createChar(7, retarrow);
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





		for(;;)
		{
			xTaskNotifyWait( 	0x00,      /* Don't clear any notification bits on entry. */
								0xffffffff , /* Reset the notification value to 0 on exit. */
								&ulNotifiedValue, /* Notified value pass out in
												  ulNotifiedValue. */
								portMAX_DELAY );  /* Block indefinitely. */



//
//		HAL_GPIO_WritePin(lamp_01_GPIO_Port, lamp_01_Pin, GPIO_PIN_SET);
//		osDelay(100);
//		HAL_GPIO_WritePin(lamp_01_GPIO_Port, lamp_01_Pin, GPIO_PIN_RESET);
						Onboard_led_ON();
						osDelay(20);
						Onboard_led_OFF();
		}
}
