/*! \file clock.h \brief header file for analog clock. */
//*****************************************************************************
//
// File Name	: 'clock.h'
// Title		: analog clock
// Date			: 14/10/2007
// Version		: 0.1
//         
// This code is distributed under the GNU Public License
//		which can be found at http://www.gnu.org/licenses/gpl.txt
//
//*****************************************************************************

#ifdef __cplusplus
extern "C" {
#endif

#ifndef CLOCK_H
#define CLOCK_H

#include <inttypes.h>

//! current time [hours, minuts, secs] on analog clock on position[middle_x, middle_y] with defined [color]
void paint_clock(uint8_t middle_x, uint8_t middle_y, uint8_t radius, uint8_t hours, uint8_t minutes, uint8_t secs, int8_t color);

#endif 

#ifdef __cplusplus
}
#endif
