/*! \file lh115baconf.h \brief Quadrature Encoder driver configuration. */
//*****************************************************************************
//
// File Name	: 'lh115baconf.h'
// Title		: LCD display pins assigment
// Author		: Michal Harakal - Copyright (C) 2007
// Created		: 2003.08.30
// Revised		: 2003.08.30
// Version		: 0.1
// Target MCU	: Atmel AVR Series
//

#ifdef __cplusplus
extern "C" {
#endif




#include <inttypes.h>

#ifndef LH115BACONF_H
#define LH115BACONF_H

#define DATAPORT     PORTA
#define DATA_DDR     DDRA
#define CTRLPORT     PORTD
#define CTRL_DDR     DDRD

#define RS        PD7
#define _WR       PD6
#define LEDBL     PD5


#endif


#ifdef __cplusplus
}
#endif