//*****************************************************************************
//
// File Name	: 'keyboardconf.h'
// Title		: cheesboard LCD Modul - Matrix Keyboard configuration
// Author		: Michal Harakal, Trautheim Technology Center (C) 2005
// Created		: 2005.10.03
// 
// This code is distributed under the GNU Public License
//		which can be found at http://www.gnu.org/licenses/gpl.txt
//
//*****************************************************************************

#ifdef __cplusplus
extern "C" {
#endif


#ifndef KEYBOARDCONF_H
#define KEYBOARDCONF_H

#define COLS     4
#define ROWS     4

/*
 * every pin is defined separated
 *      cols
 * rows 1 4 7 *
 *      2 5 8 0
 *      3 6 9 #
 *      A B C D
 */

// rows
#define ROW0_DDR   DDRD	        
#define ROW0_PORT  PORTD	        
#define ROW0_PIN   PD3

#define ROW1_DDR   DDRD	        
#define ROW1_PORT  PORTD	        
#define ROW1_PIN   PD2          

#define ROW2_DDR   DDRD	        
#define ROW2_PORT  PORTD	        
#define ROW2_PIN   PD1

#define ROW3_DDR   DDRD
#define ROW3_PORT  PORTD
#define ROW3_PIN   PD0
// cols
#define COL0_DDR     DDRD
#define COL0_PORT    PORTD	        
#define COL0_PORTIN  PIND	        
#define COL0_PIN     PD7

#define COL1_DDR     DDRD
#define COL1_PORT    PORTD
#define COL1_PORTIN  PIND	       
#define COL1_PIN     PD6

#define COL2_DDR     DDRD
#define COL2_PORT    PORTD
#define COL2_PORTIN  PIND 
#define COL2_PIN     PD5

#define COL3_DDR     DDRD
#define COL3_PORT    PORTD
#define COL3_PORTIN  PIND	        
#define COL3_PIN     PD4

// function keys
#define FUNC_KEYS_DDRB DDRB
#define FUNC_KEYS_PORT PORTB
#define FUNC_KEYS_PIN  PINB	  

#endif 

#ifdef __cplusplus
}
#endif
