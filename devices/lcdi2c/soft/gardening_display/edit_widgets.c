/*! \file edit_widgets.c \brief widgets for user input */
//*****************************************************************************
//
// File Name	: 'edit_widgets.c'
// Title		: various widgets
// Date			: 29/09/2007
// Version		: 0.2
//         
// History      : 03/10/2007 range
//
// This code is distributed under the GNU Public License
//		which can be found at http://www.gnu.org/licenses/gpl.txt
//
//*****************************************************************************

#include <stdio.h>
#include <string.h>
#include <inttypes.h>

# define WINDOW_SIZE_X 80
# define WINDOW_SIZE_Y 40

#include "gd_driver.h"
#include "graphics.h"
#include "widgets.h"
#include "keyboard.h"

uint16_t IntValuesWindow(char *title, uint16_t min, uint16_t max, uint16_t value)
{
	uint16_t old_value = value;
	
	// clear screen	
	gd_hw_clear_scr();
	
    char *curr_menu[4] = {"ESC" , "+" , "-", "OK"};
    
    uint8_t i, j, x, y;
    char value_str[10];
	uint8_t len;
	x = 0;	              
	y = (64 / 2) - (WINDOW_SIZE_Y / 2);
    
    for (;  ; )
    {
	
	
	// clear window
	for (i = 0; i < WINDOW_SIZE_Y; i++)
	  gLine(x, y + i, x + WINDOW_SIZE_X, y + i, 0);	
	// frame
	gLine(x, y, x + WINDOW_SIZE_X, y, 1);	
	gLine(x, y + WINDOW_SIZE_Y, x + WINDOW_SIZE_X, y  + WINDOW_SIZE_Y, 1);	
	gLine(x, y, x, y + WINDOW_SIZE_Y, 1);	
	gLine(x + WINDOW_SIZE_X, y, x + WINDOW_SIZE_X, y + WINDOW_SIZE_Y, 1);	
	// window title	
	Bar(x + 1, y + WINDOW_SIZE_Y - 10, WINDOW_SIZE_X - 2, 10);
    ShowStr(x + 2, y + WINDOW_SIZE_Y - 10, title, -1);  	
    
    // buttons
    for (i = 0; i < 4; i++)	
	  if (curr_menu[3 - i][0])	  
	    MenuButton(94, i * 16, curr_menu[3 - i], 0);		 
	    
	// value	
	sprintf(value_str, "%d", value);
	len = strlen(value_str) * 6;
	ShowStr(x + (WINDOW_SIZE_X  / 2)  - (len / 2), y + 10, value_str, 1);     
    
    
    gd_hw_update();	
    
    
       if( get_key_press( 1<<KEY7 )) 
         return value;
       if( get_key_press( 1<<KEY4 )) 
         return old_value;  
       if( get_key_press( 1<<KEY5 )) {       	
          if (value < max)
            value++;    
          else
            value = min;                      
       }  
       if( get_key_press( 1<<KEY6 )) 
         if (value > min)
            value--;    
          else
            value = max;                    
         keyboard_clear();   
    }
        
    
    return value;
}

