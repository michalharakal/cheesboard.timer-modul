/*! \file bresenham.h \brief bresenham algoritmus for inetegr line painting. */
//*****************************************************************************
//
// File Name	: 'bresenham.c'
// Title		: bresenham algoritmus for integer line paint
// Date			: 29/09/2007
// Version		: 0.2
//         
// History      : 29/09/2007 taken from alfi project
//
// This code is distributed under the GNU Public License
//		which can be found at http://www.gnu.org/licenses/gpl.txt
//
//*****************************************************************************

#include <inttypes.h>

//! line start
void bresenham_init (uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2);

//! increment [MoveX] and [MoveY] for the next point
int8_t bresenham_next (int8_t *MoveX, int8_t *MoveY);
