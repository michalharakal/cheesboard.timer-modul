/*! \file graphics.h \brief header file for high level graphics functions. */
//*****************************************************************************
//
// File Name	: 'graphics.h'
// Title		: header file for high level graphics functions (LCD display independent)
// Author		: Michal Harakal - Copyright (C) 2007
// Date			: 08/30/2007
// Revised		: 08/30/2007
// Version		: 0.1
// Target MCU	: Atmel AVR
//
// This code is distributed under the GNU Public License
//		which can be found at http://www.gnu.org/licenses/gpl.txt
//*****************************************************************************

#ifndef GD_GRAPHICS_H
#define GD_GRAPHICS_H

#include "gd_driver.h"

extern disp_t dd;

//! Clear the display
void gClear(void);

//! init display
void gInit(void);

//! paint line with bresenham algoritmus starting on [x1][y1] to [x2][y2]
void  gLine(int x1, int y1, int x2, int y2, int8_t color);

//! show char
void ShowChr(uint8_t x, uint8_t y, int8_t ch, int8_t color);

//! show text
void ShowStr(uint8_t x, uint8_t y, char *str, int8_t color);

//! show big text
void ShowBigStr(uint8_t x, uint8_t y, char *str, int8_t color);


#endif
