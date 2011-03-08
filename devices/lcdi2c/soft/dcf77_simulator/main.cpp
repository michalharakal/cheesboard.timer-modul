/*! \file main.c \brief ui2 modul main. */
//*****************************************************************************
//
// File Name	: 'main.c'
// Title		: ui2 modul main
// Author		: Michal Harakal - Copyright (C) 2007
// Date			: 08/30/2007
// Revised		: 08/30/2007
// Version		: 0.1
// Target MCU	: Atmel AVR
//
// This code is distributed under the GNU Public License
//		which can be found at http://www.gnu.org/licenses/gpl.txt
//
//*****************************************************************************

#include <stdio.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>     
#include "dcf77_sim.h"
#include "../common/uart.h"
#include "../../soft/common/keyboardconf.h"
#include "../../soft/common/keyboard.h"
#include "../../soft/common/gd_driver.h"
#include "../../soft/common/graphics.h"
#include "../../soft/common/widgets.h"
#include "../../soft/common/menus.h"
#include "../../soft/common/clock.h"
#include "../../soft/common/keyboard.h"
#include "../../soft/common/edit_widgets.h"

#define ALL_KEYS        (1<<KEY0 | 1<<KEY1 | 1<<KEY2 | 1<<KEY3 | 1<<KEY4 | 1<<KEY5 | 1<<KEY6 | 1<<KEY7)
 
#define REPEAT_MASK     ALL_KEYS    
#define REPEAT_START    50                        // after 500ms
#define REPEAT_NEXT     20                        // every 200ms


const char *main_menu[8] = {"", "", "Minut", "Hour", "WeDay", "Day", "Month","Year"};

static uint8_t menu = 0;
DCF77Simulator dcf77_sim(PC3);

uint8_t hours, min, sec, turbo;
uint8_t year, month, day, dow;

static uint16_t cycles_counter = 4;
static uint8_t paint = 0;
static uint8_t changed = 0;

/** 
 * timer handler (10ms Takt)
 * 
 * @param SIG_OVERFLOW0 
 * 
 * @return 
 */
SIGNAL (SIG_OVERFLOW0)    
{ 
  // reload timer with initial value
  // 255 - 144
  TCNT0 = 111;	
 
  // debounce keyboard
  static uint8_t ct0, ct1, rpt;
  uint8_t i; 

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

  cycles_counter--;
  if (cycles_counter == 0) {
    cycles_counter = 4;
	paint = 1;
  }	
}

void update_clock(void)
{
  char auxstr[30];
  sprintf(auxstr, "%.2d:%.2d:%.2d", hours, min, sec);
  ShowBigStr(10, 10, auxstr, -1);
  sprintf(auxstr, "%.2d-%.2d-%.2d %.2d", day, month, year, dow);
  ShowStr(10, 1, auxstr, -1);
}  

uint8_t UiCallback(void)
{    
    if( get_key_press( 1<<KEY0 )) 
       menu = 1;
    if( get_key_press( 1<<KEY1 )) 
       menu = 2;
	if( get_key_press( 1<<KEY3 )) 
       menu = 7;   
    if( get_key_press( 1<<KEY7 )) 
       menu = 3;
    if( get_key_press( 1<<KEY6 )) 
       menu = 4;
    if( get_key_press( 1<<KEY5 )) 
       menu = 5;
    if( get_key_press( 1<<KEY4 )) 
       menu = 6;


      
	keyboard_clear();       

    // handle user key press	 
    switch (menu)
    {    	
    	case 0:           
		  if (paint) {
		    gd_hw_clear_scr();  
    	            RepaintMenu();          
		    sec = dcf77_sim.get_secs(); 
		    update_clock();
		    gd_hw_update();	    
		    paint = 0;			
	      }		
    	  break;
    	case 1: // hours
		  dcf77_sim.get_time(&hours, &min);
    	  hours = IntValuesWindow(main_menu[3], 0, 23, hours);
		  changed = 1;
		  dcf77_sim.set_time(hours, min);
    	  gd_hw_clear_scr();  
    	  RepaintMenu();          
		  sec = dcf77_sim.get_secs(); 
          update_clock();
		  gd_hw_update();	    		  
		  menu = 0;
		  return 0;
    	  break;    	    
    	case 2: // minutes
		  dcf77_sim.get_time(&hours, &min);
    	  min = IntValuesWindow(main_menu[2], 0, 59, min);
		  dcf77_sim.set_time(hours, min);  
          changed = 1;
		  gd_hw_clear_scr();  
    	  RepaintMenu();          
		  sec = dcf77_sim.get_secs(); 
          update_clock();
		  gd_hw_update();	    
		  keyboard_clear();     
		  menu = 0;
		  return 0;
    	  break;    	    
    	case 3: // year
	  dcf77_sim.get_date(&year, &month, &day, &dow);	
    	  year = IntValuesWindow(main_menu[7], 0, 99, year);
		  dcf77_sim.set_date(year, month, day, dow);	
          changed = 1;
		  gd_hw_clear_scr();  
    	  RepaintMenu();          
		  sec = dcf77_sim.get_secs(); 
          update_clock();
		  gd_hw_update();	    
		  keyboard_clear();     
		  menu = 0;
		  return 0;
    	  break;    	  
        case 4: // month
	  dcf77_sim.get_date(&year, &month, &day, &dow);	
    	  month = IntValuesWindow(main_menu[6], 1, 12, month);
		  dcf77_sim.set_date(year, month, day, dow);	
          changed = 1;
		  gd_hw_clear_scr();  
    	  RepaintMenu();          
		  sec = dcf77_sim.get_secs(); 
          update_clock();
		  gd_hw_update();	    
		  keyboard_clear();     
		  menu = 0;
		  return 0;
    	  break;    	      
        case 5: // day
	  dcf77_sim.get_date(&year, &month, &day, &dow);	
    	  day = IntValuesWindow(main_menu[5], 1, 31, day);
		  dcf77_sim.set_date(year, month, day, dow);	
          changed = 1;
		  gd_hw_clear_scr();  
    	  RepaintMenu();          
		  sec = dcf77_sim.get_secs(); 
          update_clock();
		  gd_hw_update();	    
		  keyboard_clear();     
		  menu = 0;
		  return 0;
    	  break;    	      
    	case 6: // dow
	  dcf77_sim.get_date(&year, &month, &day, &dow);	
    	  dow = IntValuesWindow(main_menu[4], 0, 6, dow);
		  dcf77_sim.set_date(year, month, day, dow);	
          changed = 1;
		  gd_hw_clear_scr();  
    	  RepaintMenu();          
		  sec = dcf77_sim.get_secs(); 
          update_clock();
		  gd_hw_update();	    
		  keyboard_clear();     
		  menu = 0;
		  return 0;
    	  break;    	    
		case 7: // turbo
	      turbo = dcf77_sim.get_turbo_mode();	
    	  turbo = IntValuesWindow(main_menu[0], 1, 10, turbo);
		  dcf77_sim.set_turbo_mode(turbo);	
          changed = 1;
		  gd_hw_clear_scr();  
    	  RepaintMenu();          
		  sec = dcf77_sim.get_secs(); 
          update_clock();
		  gd_hw_update();	    
		  keyboard_clear();     
		  menu = 0;
		  return 0;
    	  break;    	      
    	default:
    	  menu = 0;    	   
    }      
    
    
  return 1;
}

int main (void)
{ 
  DDRC |= (1 << PC3); 
  // 02:03 27.10.1996
//  dcf77_sim.set_date(96, 10, 27, 3);
  dcf77_sim.set_date(9, 6, 16, 2);
  dcf77_sim.set_time(17,11);
  // matrix keyboard
  InitKeyborad();
  // debounced keys
  // debounce timer  
  TIMSK |= (1<<TOIE0); 		// enables the T/C0 overflow interrupt   
  TCNT0 = 111;					// start value of T/C0   
  TCCR0 = (1<<CS02)|(1<<CS00); //8bit counter prescale ck/1024  
  TIFR = (1<<ICF1);			// clear ICF1 /clear pending interrupts

  uart_init( 47 );  // 19200@14.7456
 
  InitFunctionKeys();
  // enable interrupts
  sei();
  //  display driver init
  gInit();	  
  // backlight on
  gd_hw_backlight(1);  
  
  // main menu
  AssignMenu(main_menu, "DCF77 Simulator V0.3");
  gd_hw_clear_scr();  
  RepaintMenu();          
  gd_hw_update();	 

  dcf77_sim.register_uiCallback(UiCallback); 
  dcf77_sim.get_time(&hours, &min);
  dcf77_sim.get_date(&year, &month, &day, &dow);	
    
  changed = 0;	
  dcf77_sim.reset();
  for(;;) {                
    dcf77_sim.bits_send = 0;                       
    if (changed) {
	  dcf77_sim.reset();
	  changed = 0;
	}  
    dcf77_sim.simulate();	
//    dcf77_sim.TestMuster();
    dcf77_sim.IncMinute();
    dcf77_sim.get_time(&hours, &min);    
  }
}
