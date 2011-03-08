//*****************************************************************************
// File Name	: 'keyboard.c'
// Title		: matrix 4x4 keyboard and functions keys reader 
// Author		: Michal Harakal, Trautheim Technology Center (C) 2005
// Created		: 2005.10.23
// Target MCU	: Atmel AVR Series
// History		: 14.10.2007 - added debounced keys support
//                debounced kyes from http://www.mikrocontroller.net/articles/AVR-GCC-Tutorial#.28Tasten-.29Entprellung
//
// This code is distributed under the GNU Public License
//		which can be found at http://www.gnu.org/licenses/gpl.txt
//
//*****************************************************************************

#include <avr/io.h>
#include <avr/interrupt.h>

#include "avrlibsdefs.h"
// include matrix 4x4 keyboard configuration file
#include "keyboardconf.h"
#include "keyboard.h"

#ifndef F_CPU
/* wenn F_CPU nicht bereits vorher definiert (z.B. durch
   Übergabe als Parameter zum Compiler innerhalb des Makefiles) */
#define F_CPU 14745600UL     /* Quarz mit 14.7456 Mhz */
#endif

#define ALL_KEYS        (1<<KEY0 | 1<<KEY1 | 1<<KEY2 | 1<<KEY3 | 1<<KEY4 | 1<<KEY5 | 1<<KEY6 | 1<<KEY7)
 
#define REPEAT_MASK     ALL_KEYS    
#define REPEAT_START    50                        // after 500ms
#define REPEAT_NEXT     20                        // every 200ms



uint8_t key_state;                                // debounced and inverted key state:
                                                  // bit = 1: key pressed
uint8_t key_press;                                // key press detect
 
uint8_t key_rpt;                                  // key long press and repeat
 
 
ISR( TIMER0_OVF_vect )                            // every 10ms
{
  static uint8_t ct0, ct1, rpt;
  uint8_t i;
 
  TCNT0 = (uint8_t)(int16_t)-(F_CPU / 1024 * 10e-3 + 0.5);  // preload for 10ms
 
  i = key_state ^ ~KEY_PIN;                       // key changed ?
  ct0 = ~( ct0 & i );                             // reset or count ct0
  ct1 = ct0 ^ (ct1 & i);                          // reset or count ct1
  i &= ct0 & ct1;                                 // count until roll over ?
  key_state ^= i;                                 // then toggle debounced state
  key_press |= key_state & i;                     // 0->1: key press detect
 
  if( (key_state & REPEAT_MASK) == 0 )            // check repeat function
     rpt = REPEAT_START;                          // start delay
  if( --rpt == 0 ){
    rpt = REPEAT_NEXT;                            // repeat delay
    key_rpt |= key_state & REPEAT_MASK;
  }
}
 
///////////////////////////////////////////////////////////////////
//
// check if a key has been pressed. Each pressed key is reported
// only once
//
uint8_t get_key_press( uint8_t key_mask )
{
  cli();                                          // read and clear atomic !
  key_mask &= key_press;                          // read key(s)
  key_press ^= key_mask;                          // clear key(s)
  sei();
  return key_mask;
}
 
///////////////////////////////////////////////////////////////////
//
// check if a key has been pressed long enough such that the
// key repeat functionality kicks in. After a small setup delay
// the key is reported beeing pressed in subsequent calls
// to this function. This simulates the user repeadiately
// pressing and releasing the key.
//
uint8_t get_key_rpt( uint8_t key_mask )
{
  cli();                                          // read and clear atomic !
  key_mask &= key_rpt;                            // read key(s)
  key_rpt ^= key_mask;                            // clear key(s)
  sei();
  return key_mask;
}
 
///////////////////////////////////////////////////////////////////
//
uint8_t get_key_short( uint8_t key_mask )
{
  cli();                                          // read key state and key press atomic !
  return get_key_press( ~key_state & key_mask );
}
 
///////////////////////////////////////////////////////////////////
//
uint8_t get_key_long( uint8_t key_mask )
{
  return get_key_press( get_key_rpt( key_mask ));
}

void keyboard_clear(void) 
{	
    key_press = 0;
}



/*
 * inits keyboard IO, columns are set as inputs with pullups set
 */
void InitKeyborad()
{
  // columns as inputs with pullup
  cbi(COL0_DDR, COL0_PIN);
  sbi(COL0_PORT, COL0_PIN);
  cbi(COL1_DDR, COL1_PIN);
  sbi(COL1_PORT, COL1_PIN);
  cbi(COL2_DDR, COL2_PIN);
  sbi(COL2_PORT, COL2_PIN);
  cbi(COL3_DDR, COL3_PIN);
  sbi(COL3_PORT, COL3_PIN);
  // rows as output
  sbi(ROW0_DDR, ROW0_PIN);
  sbi(ROW1_DDR, ROW1_PIN);
  sbi(ROW2_DDR, ROW2_PIN);
  sbi(ROW3_DDR, ROW3_PIN);
}

void ActivateRow(uint8_t row)
{
  // set all rows as inputs with pullups
  cbi(ROW0_DDR,  ROW0_PIN);
  sbi(ROW0_PORT, ROW0_PIN);

  cbi(ROW1_DDR,  ROW1_PIN);
  sbi(ROW1_PORT, ROW1_PIN);

  cbi(ROW2_DDR,  ROW2_PIN);
  sbi(ROW2_PORT, ROW2_PIN);

  cbi(ROW3_DDR,  ROW3_PIN);
  sbi(ROW3_PORT, ROW3_PIN);
  // activate desired row as output
  switch (row) {
    case 0:
      sbi(ROW0_DDR,  ROW0_PIN);
      cbi(ROW0_PORT, ROW0_PIN);
      break;
    case 1:
      sbi(ROW1_DDR,  ROW1_PIN);
      cbi(ROW1_PORT, ROW1_PIN);
      break;
    case 2:
      sbi(ROW2_DDR,  ROW2_PIN);
      cbi(ROW2_PORT, ROW2_PIN);
      break;
    case 3:
      sbi(ROW3_DDR,  ROW3_PIN);
      cbi(ROW3_PORT, ROW3_PIN);
      break;
  }
}

/*
 * scan every row separate
 * LSB = 1. key in the 1.row
 * MSB = 4. key in the 4.row
 * 
 * read key is stored in global variablecurr_keys
 */
uint16_t ReadKeyboard(void)
{
  uint8_t row;
  uint16_t keys;
    
  keys = 0;
  row = 0;
  for (row=0; row < ROWS; row++) {
    ActivateRow(row);
    // read input and set proper bits
    if (bit_is_clear(COL0_PORTIN, COL0_PIN))
      keys |=  (0x01 << (row * 4));
    if (bit_is_clear(COL1_PORTIN, COL1_PIN))
      keys |=  (0x02 << (row * 4));
    if (bit_is_clear(COL2_PORTIN, COL2_PIN))
      keys |=  (0x04 << (row * 4));
    if (bit_is_clear(COL3_PORTIN, COL3_PIN))
      keys |=  (0x08 << (row * 4));
  }
  return keys;
}

/*
 * check doubonced keys
 */
void InitFunctionKeys(void)
{ 
  // inputs	 
  FUNC_KEYS_DDRB = 0x00; 
  // pullups
  FUNC_KEYS_PORT = 0xFF;
  // debounce timer  
  TCCR0 = (1<<CS02)|(1<<CS00);			// divide by 1024
  TIMSK = 1<<TOIE0;      				// enable timer interrupt
}
