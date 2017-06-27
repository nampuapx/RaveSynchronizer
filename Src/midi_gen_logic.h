/*
 * midi_gen_logic.h
 *
 *  Created on: 15 θών. 2017 γ.
 *      Author: koko
 */

#ifndef MIDI_GEN_LOGIC_H_
#define MIDI_GEN_LOGIC_H_


typedef enum{
	internal_clock_and_transport,
	external_clock_and_transport

}_ext_int_state;

extern _ext_int_state ext_int_state;


extern uint8_t MIDI_start_status;
extern uint16_t MIDI_restart_value;
extern uint8_t need_start;
extern _ext_int_state ext_int_state;

#endif /* MIDI_GEN_LOGIC_H_ */
