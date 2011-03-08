#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdlib.h>
#include <inttypes.h>

#include "keys.h"

#define REPEAT_MASK	0     	        // repeat: no key
#define REPEAT_START	50		// after 500ms
#define REPEAT_NEXT	20		// every 200ms

#define KEY_PIN		PINB

static uint8_t key_state;		// debounced and inverted key state:
volatile uint8_t key_press;		// key press detect
static uint8_t key_rpt;	         	// key long press and repeat

uint8_t get_key_press( uint8_t key_mask )
{
  cli();					// read and clear atomic !
  key_mask &= key_press;                        // read key(s)
  key_press ^= key_mask;                        // clear key(s)
  sei();
  return key_mask;
}

uint8_t get_key_rpt( uint8_t key_mask )
{
  cli();					// read and clear atomic !
  key_mask &= key_rpt;                        	// read key(s)
  key_rpt ^= key_mask;                        	// clear key(s)
  sei();
  return key_mask;
}
 
 
uint8_t get_key_short( uint8_t key_mask )
{
  cli();			// read key state and key press atomic !
  return get_key_press( ~key_state & key_mask );
}
 
 
uint8_t get_key_long( uint8_t key_mask )
{
  return get_key_press( get_key_rpt( key_mask ));
}

void KeyDebounceTimerHandler(void) 
{
  static uint8_t ct0, ct1, rpt;
  uint8_t i;
 
  i = key_state ^ ~KEY_PIN;		// key changed ?
  ct0 = ~( ct0 & i );			// reset or count ct0
  ct1 = ct0 ^ (ct1 & i);		// reset or count ct1
  i &= ct0 & ct1;			// count until roll over ?
  key_state ^= i;			// then toggle debounced state
  key_press |= key_state & i;		// 0->1: key press detect
 
  if( (key_state & REPEAT_MASK) == 0 )	// check repeat function
     rpt = REPEAT_START;		// start delay
  if( --rpt == 0 ){
    rpt = REPEAT_NEXT;			// repeat delay
    key_rpt |= key_state & REPEAT_MASK;
  }
}

