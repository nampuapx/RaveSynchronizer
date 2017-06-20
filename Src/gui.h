/*
 * gui.h
 *
 *  Created on: 16 θών. 2017 γ.
 *      Author: koko
 */

#ifndef GUI_H_
#define GUI_H_


#define SIMBOL_LCD_WIDTH	17
#define SIMBOL_LCD_ROWS		2



typedef struct{
	uint8_t xy;
	char txt[SIMBOL_LCD_WIDTH];
}type_q_lcd_element;


void gui_print_lcd_bpm(void);


#endif /* GUI_H_ */
