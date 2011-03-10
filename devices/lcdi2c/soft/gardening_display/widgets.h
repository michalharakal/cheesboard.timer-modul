/*! \file widget.h \brief bresenham algoritmus for inetegr line painting. */
//*****************************************************************************
//
// File Name	: 'widgets.h'
// Title		: various widgets
// Date			: 29/09/2007
// Version		: 0.2
//         
// History      : 03/10/2007 bar
//
// This code is distributed under the GNU Public License
//		which can be found at http://www.gnu.org/licenses/gpl.txt
//
//*****************************************************************************

#ifndef GD_WIDGETS_H
#define GD_WIDGETS_H


#include <inttypes.h>

//! bar widget on the position of left [x,y]
void Bar(int8_t x, int8_t y, uint8_t sizex, uint8_t sizey);

//! value bar widget on the position of left [x,y]
void ValueBar(int8_t x, int8_t y, uint8_t sizex, uint8_t sizey, int16_t max, int16_t value);

//! value bar with text widget on the position of left [x,y]
void ValueBarTxt(int8_t x, int8_t y, uint8_t size, int16_t max, int16_t value);

//! menu button
void MenuButton(int8_t x, int8_t y, char *txt, uint8_t pressed);

#endif
