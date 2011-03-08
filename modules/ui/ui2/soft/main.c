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

#include <avr/io.h>
#include <avr/interrupt.h>

#ifndef F_CPU
/* wenn F_CPU nicht bereits vorher definiert (z.B. durch
   Übergabe als Parameter zum Compiler innerhalb des Makefiles) */
#define F_CPU 14745600UL     /* Quarz mit 14.7456 Mhz */
#endif

#include <util/delay.h>     

#include "../../soft/common/gd_driver.h"
#include "../../soft/common/graphics.h"
#include "../../soft/common/widgets.h"
#include "../../soft/common/menus.h"
#include "../../soft/common/clock.h"
#include "../../soft/common/keyboard.h"
#include "../../soft/common/edit_widgets.h"

char *main_menu[8] = {"", "Stund", "Minut", "Sonne", "", "", "",""};

uint8_t menu = 0;

int main (void)
{ 
  // matrix keyboard
  InitKeyborad();
  // debounced keys
  InitFunctionKeys();
  // enable interrupts
  sei();
  //  display driver init
  gInit();	  
  // backlight on
  gd_hw_backlight(1);  
  
    // main menu
  AssignMenu(main_menu, "WIGA ver 2.0");
  
  uint8_t hours = 1, min = 28, sec = 0, sonne = 150;
    
  for(;;) {                                       
    
    gd_hw_clear_scr();  
    switch (menu)
    {
    	keyboard_clear();     
    	case 0:           
    	  RepaintMenu();
          ValueBarTxt(35, 45, 128-40, 255, sonne);
          paint_clock(80, 22, 22, hours, min, sec, 1);  
    	  break;
    	case 1: // sonne
    	  sonne = IntValuesWindow(main_menu[3], 0, 255, sonne);
    	  menu = 0;
    	  break;    	    
    	case 2: // Zeit
    	  DateValuesWindow("Anfang", &hours, &min, &sec);
    	  menu = 0;
    	  break;    	    
    	case 3:
    	  min = IntValuesWindow(main_menu[2], 0, 59, min);
    	  menu = 0;
    	  break;    	    
    	default:
    	  menu = 0;    	   
    }    
    
    if( get_key_press( 1<<KEY0 )) 
       menu = 1;
    if( get_key_press( 1<<KEY2 )) 
       menu = 2;
    if( get_key_press( 1<<KEY1 )) 
      menu = 3;
    
    gd_hw_update();	    
    keyboard_clear();     
  }
}
