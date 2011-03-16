/*! \file widget.c \brief various widgets. */
//*****************************************************************************
//
// File Name	: 'widgets.c'
// Title		: various widgets
// Date			: 29/09/2007
// Version		: 0.2
//         
// This code is distributed under the GNU Public License
//		which can be found at http://www.gnu.org/licenses/gpl.txt
//
//*****************************************************************************

#include <stdio.h>
#include <string.h>
#include <inttypes.h>

extern "C" {
#include "graphics.h"
}

//! bar widget on the position of left [x,y]
void Bar(int8_t x, int8_t y, uint8_t sizex, uint8_t sizey)
{
	uint8_t i;
	for (i = y; i < y + sizey; i++)	
		gLine(x, i, x + sizex, i, 1);	
}

//! bar widget on the position of left [x,y]
void ValueBar(int8_t x, int8_t y, uint8_t sizex, uint8_t sizey, int16_t max, int16_t value)
{	
	// frame around
	gLine(x, y, x, y + sizey, 1) ;
	gLine(x, y + sizey, x + sizex, y + sizey, 1);
	gLine(x + sizex, y + sizey, x + sizex, y, 1);
	gLine(x + sizex, y, x, y, 1);
	// value
	uint8_t value_pixels = (float)(sizex / (float)max) * value;
	uint8_t i;
	for (i = y + 1; i < (y + sizey ); i++)	
		gLine(x, i, x +  value_pixels, i, 1);	
}

//! bar widget on the position of left [x,y]
void ValueBarTxt(int8_t x, int8_t y, uint8_t size, int16_t max, int16_t value)
{	
	char value_str[10];
	switch (dd.curr_rotate) {
	  case 0: // none
	    ValueBar(x, y, size, 8, max, value);
	    sprintf(value_str, "%d", value);
	    ShowStr(x + (size / 2) - ((strlen(value_str) * 6) / 2), y - 1, value_str, -1); 
	    break;
	  case 1: // 90 Grad
	    ValueBar(x, y, size, 12, max, value);
	    break;
	}    
}

#define BUTTON_SIZE_X 32
#define BUTTON_SIZE_Y 12
void MenuButton(int8_t x, int8_t y, char *txt, uint8_t pressed)
{
	if (pressed)
	{
	  // button frame 
  	  gLine(x + 1, y, x + BUTTON_SIZE_X + 1, y, 1) ;  	
	  gLine(x + BUTTON_SIZE_X + 1, y, x + BUTTON_SIZE_X + 1, y + BUTTON_SIZE_Y - 1, 1);
	  gLine(x + BUTTON_SIZE_X + 1, y + BUTTON_SIZE_Y - 1, x + 1, y + BUTTON_SIZE_Y - 1, 1);
	  gLine(x + 1, y + BUTTON_SIZE_Y - 1, x + 1, y, 1);  
	  // shadow
	  /*
	  gLine(x + 1, y + 1, x, y + 1) ;  	
	  gLine(x, y + 1, x, y + BUTTON_SIZE_Y);
	  gLine(x, y + BUTTON_SIZE_Y, x + BUTTON_SIZE_X - 1, y + BUTTON_SIZE_Y);
	  gLine(x + BUTTON_SIZE_X - 1, y + BUTTON_SIZE_Y, x + BUTTON_SIZE_X - 1, y + BUTTON_SIZE_Y - 1);  
	  */
	  
	  ShowStr(x + 3, y + 1, txt, 1);
	}
	else 
	{	
	  // button frame 
  	  gLine(x, y + 1, x + BUTTON_SIZE_X, y + 1, 1) ;  	
	  gLine(x + BUTTON_SIZE_X, y + 1, x + BUTTON_SIZE_X, y + BUTTON_SIZE_Y,1);
	  gLine(x + BUTTON_SIZE_X, y + BUTTON_SIZE_Y, x, y + BUTTON_SIZE_Y,1);
	  gLine(x, y + BUTTON_SIZE_Y,  x, y + 1,1);  
	  // shadow frame
	  gLine(x + 1, y + 1, x + 1, y,1) ;  	
	  gLine(x + 1, y, x + BUTTON_SIZE_X, y,1);
	  gLine(x + BUTTON_SIZE_X + 1, y, x + BUTTON_SIZE_X + 1, y + BUTTON_SIZE_Y - 1,1);		
	  gLine(x + BUTTON_SIZE_X + 1, y, x + BUTTON_SIZE_X, y + BUTTON_SIZE_Y - 1,1);	
	  
	  ShowStr(x + 2, y + 2, txt, 1);
	}
} 
