/*! \file lh115ba.c \brief driver LH115BA. */
//*****************************************************************************
//
// File Name	: 'lh115ba.c'
// Title		: driver for LCD driver for LH115BA displays
// Author		: Michal Harakal - Copyright (C) 2007
// Date			: 08/27/2007
// Revised		: 08/27/2007
// Version		: 0.1
// Target MCU	: Atmel AVR
// Editor Tabs	: 4
//
// rewritten from serdisp_specific_lh155.c
// routines for controlling displays controlled by lh155-compliant controllers
// http://serdisplib.sourceforge.net/
// *
// *************************************************************************
// *
// * copyright (C) 2006-2007  wolfgang astleitner
// * email     mrwastl@users.sourceforge.net
// *
/**************************************************************************
 * This program is free software; you can redistribute it and/or modify   
 * it under the terms of the GNU General Public License as published by   
 * the Free Software Foundation; either version 2 of the License, or (at  
 * your option) any later version.                                        
 *                                                                        
 * This program is distributed in the hope that it will be useful, but    
 * WITHOUT ANY WARRANTY; without even the implied warranty of             
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU      
 * General Public License for more details.                               
 *                                                                        
 * You should have received a copy of the GNU General Public License      
 * along with this program; if not, write to the Free Software            
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA              
 * 02111-1307, USA.  Or, point your browser to                            
 * http://www.gnu.org/copyleft/gpl.html                                   
 *************************************************************************
 */

#include <avr/io.h>

#ifndef F_CPU
/* wenn F_CPU nicht bereits vorher definiert (z.B. durch
   Übergabe als Parameter zum Compiler innerhalb des Makefiles) */
#define F_CPU 14745600UL     /* Quarz mit 14.7456 Mhz */
#endif
#include <util/delay.h>     

#include "gd_driver.h"
#include "lh115baconf.h"

#define LCD_X_RES                  128
#define LCD_Y_RES                  64
#define LCD_COLS                   128 / 8 
#define LCD_CACHE_SIZE             ((LCD_X_RES * LCD_Y_RES) / 8)

static uint8_t scrbuf[LCD_CACHE_SIZE];

/*
 * command constants
 */

#define CMD_XADDRLO       0x00
#define CMD_XADDRHI       0x10
#define CMD_YADDRLO       0x20
#define CMD_YADDRHI       0x30
#define CMD_DISPSTARTLO   0x40
#define CMD_DISPSTARTHI   0x50
#define CMD_NLINEALTLO    0x60
#define CMD_NLINEALTHI    0x70
#define CMD_DISPCONTR1    0x80
#define INI_DISPOFF       0x00
#define INI_DISPON        0x01
#define INI_MODNORMAL     0x00
#define INI_MODALLON      0x02
#define CMD_DISPCONTR2    0x90
#define INI_SWAPOFF       0x00
#define INI_SWAPON        0x02
#define INI_REVERSEOFF    0x00
#define INI_REVERSEON     0x08
#define CMD_INCRCONTR     0xA0
#define INI_INCRX         0x01
#define INI_INCRY         0x02
#define INI_INCRBOTH      0x03
#define CMD_POWCONTR1     0xB0
#define INI_POWEROFF      0x00
#define INI_POWERON       0x02
#define CMD_POWCONTR2     0xD0
#define CMD_POWCONTR3     0xE0
#define CMD_RESETOFF      0xF0
#define CMD_RESETON       0xF1

void lh155_transfer(uint8_t iscmd, uint8_t item) {     
  uint8_t data_flag = 0;

  if (iscmd != 0)
    data_flag = (1 << RS);

/*===data 
CS  RES RS  WR   RD
|    #   |   #    #     |   |   |   |   #   #   #   #    
|    #   |   |    #     |   |   |   |   #   #   #   #    
|    #   |   #    #     |   |   |   |   #   #   #   #    
=== cmd 
|    #   #   #    #     |   |   |   |   |   |   #   #    
|    #   #   |    #     |   |   |   |   |   |   #   #    
|    #   #   #    #     |   |   |   |   |   |   #   #    
*/

//======= 1. CLK
  DATAPORT = item;  
  CTRLPORT &= ~(1 << RS);
  CTRLPORT |= (1 << _WR) | data_flag;
//======= 2. CLK
  CTRLPORT &= ~(1 << _WR);    
//======= 3. CLK
  CTRLPORT |= (1 << _WR);    
}

void lh155_writecmd(uint8_t cmd) {
  lh155_transfer(1, cmd);
}


void lh155_writedata(uint8_t data) {
  lh155_transfer(0, data);
}


//! Initialize the display, clear it, and prepare it for access
void lh155_init(void)
{
  lh155_writecmd (CMD_RESETOFF);
  lh155_writecmd (CMD_XADDRLO);
  lh155_writecmd (CMD_YADDRLO);
  lh155_writecmd (CMD_YADDRHI);
  lh155_writecmd (CMD_DISPSTARTLO);
  lh155_writecmd (CMD_DISPSTARTHI);
  lh155_writecmd (CMD_NLINEALTLO | 0x01);
  lh155_writecmd (CMD_NLINEALTHI | 0x00);
  lh155_writecmd (CMD_RESETOFF);
  lh155_writecmd (CMD_DISPCONTR1 | INI_DISPON);
  lh155_writecmd (CMD_DISPCONTR2 | INI_SWAPON);
  lh155_writecmd (CMD_INCRCONTR  | INI_INCRBOTH);
  lh155_writecmd (CMD_POWCONTR1  | INI_POWERON);
  lh155_writecmd (CMD_POWCONTR2  | 0x0E);
  lh155_writecmd (CMD_POWCONTR3);
  lh155_writecmd (CMD_RESETON);
  lh155_writecmd (CMD_POWCONTR3);
  lh155_writecmd (CMD_RESETOFF);    
}

void gd_hw_init(disp_t *dd)
{  
  dd->width             = 128; 
  dd->height            = 64; 
  dd->depth             = 1;   
  dd->feature_backlight = 0; 
  dd->feature_invert    = 1; 
  dd->curr_invert       = 0;
  dd->delay             = 0;      
  
  // define outputs
  DATA_DDR = 0xFF; 
  CTRL_DDR = (1 << RS) | (1 << _WR) | (1 << LEDBL);
  
  lh155_init();
}  

//! update the display
void gd_hw_update(void)
{
  uint8_t i, col;
  uint8_t data;
  
  for (i = 0; i < 64; i++) {

    lh155_writecmd (CMD_YADDRLO | (i & 0x0F));
    lh155_writecmd (CMD_YADDRHI | (i >> 4));

    lh155_writecmd (CMD_XADDRLO);
    
    for (col = 0; col < LCD_COLS; col++) {
      data = scrbuf [ LCD_COLS * i + col];

      //if (dd.curr_invert && !(dd.feature_invert))
      //  data = ~data;

      lh155_writedata(data);
    }
  }
}  

//! Clear the display
void gd_hw_clear_scr(void)
{
  uint8_t i,j;
  for (i = 0; i < LCD_X_RES; i++)
    for (j = 0; j < LCD_Y_RES; j++)
      gd_hw_set_pixel(i, j, 0);  
}

//! puts pixel
void gd_hw_set_pixel(uint8_t x, uint8_t y, int8_t color)
{
  uint8_t col;
  uint8_t value_orig, value, mask;
  
  if ((x >= LCD_X_RES) || (y >= LCD_Y_RES))
     return;
  
  col = x / 8;

  value_orig = scrbuf[ LCD_COLS * y + col ];

  mask = 1 << (7-(x % 8));

  switch (color) {
  	case 0: 
  	  value = (value_orig & (~(mask & 0xFF))); 
  	  break;
  	case 1:
  	  value = (value_orig | mask); 
  	  break;
  	default:
  	  value = (value_orig ^ (0xFF & mask));   	    
  }	
    	//value = (color) ? (value_orig | mask) : (value_orig & (0xFF ^ mask));   
  
  if (value_orig != value) {
    scrbuf[ LCD_COLS * y + col ] = value;
  //  dd->scrbuf_chg[ ((cols + 7) / 8) * y_i + (col / 8)] |=  (1 << (col % 8));
  }  
}

//! reads pixel
uint8_t gd_hw_get_pixel(uint8_t x, uint8_t y)
{  
  return scrbuf[ LCD_COLS * y + x / 8 ] | 1 << (7-(x % 8));  	
}

//! backlight on if [on] is nonzero
void gd_hw_backlight(uint8_t on)
{
	if (on)	
	  CTRLPORT |= (1 << LEDBL);
	else
	  CTRLPORT &= ~(1 << LEDBL);	  
}
