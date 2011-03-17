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

#ifndef GD_DRIVER_H
#define GD_DRIVER_H

#include <inttypes.h>

#if defined( __cplusplus )
extern "C" {
#endif

/* struct containing all infos and other things concerning a display */
typedef struct {
  uint8_t width;            /* width of display in pixels */
  uint8_t height;             /* height of display in pixels */
  uint8_t depth;              /* colour depth (1: monochrome) */  
  
  uint8_t  feature_backlight;  /* backlight available? */
  uint8_t  feature_invert;     /* is hardware inversion of display supported? */                            

  uint8_t delay;              /* delay after each write-operation (in nanoseconds).
                               ns = 0: ignore (no delay)
                               ns >= 1: delay (at least) ns nanoseconds
                               */
  uint8_t curr_rotate;        /* display normal (0) or rotated 180 degrees (1) */
  uint8_t curr_backlight;     /* status of backlight */
  uint8_t curr_invert;        /* display inverted (1) or not (0) */    
} disp_t;

//! inits display
void gd_hw_init(disp_t *dd);
//! update display from buffer
void gd_hw_update(void);
//! Clear the display
void gd_hw_clear_scr(void);
//! puts pixel
void gd_hw_set_pixel(uint8_t x, uint8_t y, int8_t color);
//! backlight on if [on] is nonzero
void gd_hw_backlight(uint8_t on);

//! inits display
void gd_init(void);
    
#if defined( __cplusplus )
}
#endif

#endif
