/**
  ******************************************************************************
  * @file           : usbd_midi_if.c
  * @brief          :
  ******************************************************************************

    (CC at)2016 by D.F.Mac. @TripArts Music

*/

/* Includes ------------------------------------------------------------------*/
#include "usbd_midi_if.h"
#include "stm32f1xx_hal.h"
#include "MIDI_lib\midi_lib.h"


//#include "queue32.h"

// basic midi rx/tx functions
static uint16_t MIDI_DataRx(uint8_t *msg, uint16_t length);
static uint16_t MIDI_DataTx(uint8_t *msg, uint16_t length);

// from mi:muz (Internal)
//stB4Arrq rxq;
//


static int checkMidiMessage(uint8_t *pMidi);

USBD_MIDI_ItfTypeDef USBD_Interface_fops_FS =
{
  MIDI_DataRx,
  MIDI_DataTx
};

static uint16_t MIDI_DataRx(uint8_t *msg, uint16_t length){
//  uint16_t cnt;
//  uint16_t msgs = length / 4;
//  uint16_t chk = length % 4;
//  if(chk == 0){
//    for(cnt = 0;cnt < msgs;cnt ++){
//      b4arrq_push(&rxq,((uint32_t *)msg)+cnt);
//    }
//  }
//  return 0;
//	led_01_TOGG();


}

void sendMidiMessage(uint8_t *msg, uint16_t size){
  if(size == 4){
//	APP_Rx_Buffer[0] = msg[0];
//	APP_Rx_Buffer[1] = msg[1];
//	APP_Rx_Buffer[2] = msg[2];
//	APP_Rx_Buffer[3] = msg[3];
//    USBD_MIDI_SendData(&hUsbDeviceFS, APP_Rx_Buffer, size);
    MIDI_DataTx(msg, size);
  }
}

static uint16_t MIDI_DataTx(uint8_t *msg, uint16_t length){
  uint32_t i = 0;
  while (i < length) {
    APP_Rx_Buffer[APP_Rx_ptr_in] = *(msg + i);
    APP_Rx_ptr_in++;
    i++;
    if (APP_Rx_ptr_in == APP_RX_DATA_SIZE) {
      APP_Rx_ptr_in = 0;
    }
  }
  return USBD_OK;
}



static uint8_t mbuffer[4];


//extern USBD_HandleTypeDef hUsbDeviceFS;
void USBMIDIsend_MIDIClock(uint8_t jack_num){
  mbuffer[0] = (jack_num<<4) | 0x0f;
  mbuffer[1] = MIDI_REAL_TIME_Clock;
  sendMidiMessage(mbuffer,4);
}
void USBMIDIsend_MIDIStart(uint8_t jack_num){
  mbuffer[0] = (jack_num<<4) | 0x0f;
  mbuffer[1] = MIDI_REAL_TIME_Start;
  sendMidiMessage(mbuffer,4);
}

//void sendNoteOn(uint8_t ch, uint8_t note, uint8_t vel){
//  buffer[0] = 0x09;
//  buffer[1] = 0x90 | ch;
//  buffer[2] = 0x7f & note;
//  buffer[3] = 0x7f & vel;
//  sendMidiMessage(buffer,4);
//}
//
//void sendNoteOff(uint8_t ch, uint8_t note){
//  buffer[0] = 0x08;
//  buffer[1] = 0x80 | ch;
//  buffer[2] = 0x7f & note;
//  buffer[3] = 0;
//  sendMidiMessage(buffer,4);
//}
//
//void sendCtlChange(uint8_t ch, uint8_t num, uint8_t value){
//  buffer[0] = 0x0b;
//  buffer[1] = 0xb0 | ch;
//  buffer[2] = 0x7f & num;
//  buffer[3] = 0x7f & value;
//  sendMidiMessage(buffer,4);
//}


