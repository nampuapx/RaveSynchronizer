/*
 * vc1228.c
 *
 *  Created on: 27 θών. 2017 γ.
 *      Author: koko
 */


#include "stm32f1xx_hal.h"
#include "cmsis_os.h"
#include "vc1228.h"

extern SPI_HandleTypeDef hspi1;
osThreadId mycur_TaskHandle;

void HAL_SPI_TxCpltCallback(SPI_HandleTypeDef *hspi)
{
	BaseType_t xHigherPriorityTaskWoken;

	xHigherPriorityTaskWoken = pdFALSE;
	xTaskNotifyFromISR(mycur_TaskHandle,
							( 1UL << 0UL ),
	                        eSetBits,
	                        &xHigherPriorityTaskWoken );
	portYIELD_FROM_ISR( xHigherPriorityTaskWoken );

}


const uint8_t WS_decode[] = {0x88,0x8C,0xC8,0xCC};


char WS2812_buff[WS2812_LENGTH*12];


void WS2812_spi_send(char * buff,uint16_t len){

	mycur_TaskHandle = xTaskGetCurrentTaskHandle();
	HAL_SPI_Transmit_DMA(&hspi1, buff, len);
	uint32_t ulNotifiedValue;
	xTaskNotifyWait(0x00,0xffffffff,&ulNotifiedValue,portMAX_DELAY);
}

void pix_decode(uint32_t color, uint8_t * buff){
	uint8_t i;
	//uint32_t mask = 3;


	for(i=0;i<24;i=i+2){
			*buff = WS_decode[color&3];
			buff++;
			color>>=2;

	}
}

void pix_send(uint32_t color){
	char buff[12];

	pix_decode(color,buff);
	WS2812_spi_send(buff,12);
}

void WS2812_buff_send(uint32_t * pixels_b, uint16_t len){

uint16_t i;
	char * to_WS_buff = WS2812_buff;


//data prepare
	for(i=0;i<len;i++){
		pix_decode(pixels_b[i],to_WS_buff);
		to_WS_buff+=12;
	}
		WS2812_spi_send(WS2812_buff,WS2812_LENGTH*12);
}



