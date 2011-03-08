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

#define WINDOW_SIZE_X 80
#define WINDOW_SIZE_Y 40

#include "gd_driver.h"
#include "graphics.h"
#include "widgets.h"
#include "keyboard.h"

#define FRAME_X    0
#define FRAME_Y    (64 / 2) - (WINDOW_SIZE_Y / 2)
uint16_t IntValuesWindow(const char *title, int16_t min, int16_t max, int16_t value)
{
  uint16_t old_value = value;
  const char *curr_menu[4] = {"ESC" , "+" , "-", "OK"};
  uint8_t i;
  char value_str[10];
  uint8_t len;

	
  // clear screen	
  gd_hw_clear_scr();
  keyboard_clear();   
    
  for ( ; ; )
  {
    // clear window
    for (i = 0; i < WINDOW_SIZE_Y; i++)
      gLine(FRAME_X, FRAME_Y + i, FRAME_X + WINDOW_SIZE_X, FRAME_Y + i, 0);	
    // frame
    gLine(FRAME_X, FRAME_Y, FRAME_X + WINDOW_SIZE_X, FRAME_Y, 1);	
    gLine(FRAME_X, FRAME_Y + WINDOW_SIZE_Y, FRAME_X + WINDOW_SIZE_X, FRAME_Y  + WINDOW_SIZE_Y, 1);	
    gLine(FRAME_X, FRAME_Y, FRAME_X, FRAME_Y + WINDOW_SIZE_Y, 1);	
    gLine(FRAME_X + WINDOW_SIZE_X, FRAME_Y, FRAME_X + WINDOW_SIZE_X, FRAME_Y + WINDOW_SIZE_Y, 1);	
    // window title	
    Bar(FRAME_X + 1, FRAME_Y + WINDOW_SIZE_Y - 10, WINDOW_SIZE_X - 2, 10);
    ShowStr(FRAME_X + 2, FRAME_Y + WINDOW_SIZE_Y - 10, title, -1);  	
    
    // buttons
    for (i = 0; i < 4; i++)	
      if (curr_menu[3 - i][0])	  
        MenuButton(94, i * 16, curr_menu[3 - i], 0);		 
   // value
   sprintf(value_str, "%d", value);
   len = strlen(value_str) * 6;
   ShowStr(FRAME_X + (WINDOW_SIZE_X  / 2)  - (len / 2), FRAME_Y + 10, value_str, 1);     
    
   gd_hw_update();	    
        
    if( get_key_press( 1<<OK_KEY )) 
       return value;
    if( get_key_press( 1<<ESC_KEY )) 
       return old_value;  
    if ( (get_key_press(1<<PLUS_KEY )) | (get_key_rpt( 1<<PLUS_KEY )))
    {       	
      if (value < max)
        value++;    
      else
        value = min;                      
    }  
    if ( (get_key_press(1<<MINUS_KEY )) | (get_key_rpt( 1<<MINUS_KEY )))
    {
      if (value > min)
        value--;    
      else
        value = max;                          
    }    
  }  
}

uint8_t TimeValuesWindow(const char *title, uint8_t *hours, uint8_t *minutes, uint8_t *seconds)
{  
  const char *time_menu[4] = {"Std.", "Min.", "Sec.",""};
  const char *keys_menu[4] = {"ESC", "", "","OK"};
  uint8_t i, old_values[3], len;
  char value_str[10];

  old_values[0] = *hours;
  old_values[1] = *minutes;
  old_values[2] = *seconds;

  // clear screen	
  gd_hw_clear_scr();
  for ( ; ; )
  {    
    // left buttons
    for (i = 0; i < 4; i++)	
      if (time_menu[3 - i][0])	  
        MenuButton(0, i * 16, time_menu[3 - i], 0);		 
    // right buttons
    for (i = 0; i < 4; i++)	
      if (keys_menu[3 - i][0])	  
        MenuButton(94, i * 16, keys_menu[3 - i], 0);		 

    // time    
    sprintf(value_str, "%.2d:%.2d:%.2d", old_values[0], old_values[1], old_values[2]);
    len = strlen(value_str) * 6;
    ShowStr((WINDOW_SIZE_X  / 2), 28, value_str, 1);         
    gd_hw_update();	
        
    if( get_key_press( 1<<KEY0 ))  // hours
    {
      old_values[0] = IntValuesWindow(time_menu[0], 0, 24, old_values[0]);
      gd_hw_clear_scr();
    }  
    if( get_key_press( 1<<KEY1 ))  // minutes
    {
      old_values[1] = IntValuesWindow(time_menu[1], 0, 59, old_values[1]);
      gd_hw_clear_scr();
    }  
    if( get_key_press( 1<<KEY2 ))  // seconds
    {
      old_values[2] = IntValuesWindow(time_menu[2], 0, 59, old_values[2]);
      gd_hw_clear_scr();
    } 
    if( get_key_press( 1<<ESC_KEY )) 
       return 0;  
    if( get_key_press( 1<<OK_KEY )) {
       *hours = old_values[0];
       *minutes = old_values[1];
       *seconds = old_values[2];
       return 1;  
    }
    keyboard_clear();          
  }
}
  



