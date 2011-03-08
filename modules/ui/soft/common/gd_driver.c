/*! \file gd_driver.h \brief wrapper for graphics display driver LH115BA. */
//*****************************************************************************
//
// File Name	: 'gd_driver.h'
// Title		: wrapper for LCD driver for LH115BA displays
// Author		: Michal Harakal - Copyright (C) 2007
// Date			: 08/27/2007
// Revised		: 08/27/2007
// Version		: 0.1
// Target MCU	: Atmel AVR
// Editor Tabs	: 4
//
// This code is distributed under the GNU Public License
//		which can be found at http://www.gnu.org/licenses/gpl.txt
//
//*****************************************************************************

#include <stdlib.h>

#include "gd_driver.h"

static disp_t dd;

//! inits display
void gd_init(void)
{
  // init hw specific data	 
  gd_hw_init(&dd);  
}
