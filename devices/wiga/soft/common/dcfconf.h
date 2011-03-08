/*! \file dcfconf.h \brief dcf receiver IO. */
//*****************************************************************************
//
// File Name	: 'dcfconf.h'
// Title		: LED for DCF-77 data receive
// Author		: Michal Harakal - Copyright (C) 2007
// Version		: $Id$
// Target MCU	: Atmel AVR Series
//

#include <inttypes.h>

#ifndef DCFCONF_H
#define DCFCONF_H

#define DCF_DATA_IN_PORT PIND
#define DCF_DDR       DDRD
#define DCF_DATA_PIN  PD3
#define DCF_PORT      PORTD
 
#define DCF_LED_PORT PORTC
#define DCF_LED_DDR  DDRC
#define DCF_LED_PIN  PC5

#endif
