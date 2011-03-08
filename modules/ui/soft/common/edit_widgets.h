/*! \file edit_widgets.h \brief widgets for user input */
//*****************************************************************************
//
// File Name	: 'edit_widgets.h'
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

#ifndef EDIT_WIDGETS_H
#define EDIT_WIDGETS_H

#include <inttypes.h>

extern uint16_t IntValuesWindow(char *title, uint16_t min, uint16_t max, uint16_t value);

extern uint8_t DateValuesWindow(char *title, uint8_t *hours, uint8_t *minutes, uint8_t *seconds);

#endif
