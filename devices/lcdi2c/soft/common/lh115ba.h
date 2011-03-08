/*! \file lh115ba.h \brief driver LH115BA. */
//*****************************************************************************
//
// File Name	: 'lh115ba.h'
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

#ifdef __cplusplus
extern "C" {
#endif


/* struct containing all infos and other things concerning a display */
typedef struct {
  uint8_t width;            /* width of display in pixels */
  uint8_t height;             /* height of display in pixels */
  uint8_t depth;              /* colour depth (1: monochrome) */
  uint8_t startxcol;          /* offset, if display-area is smaller than controller supported area */
  uint8_t startycol;          /*   and display area doesn't start at (0,0) (eg: nokia 7110)   */
  
  uint32_t  dsparea_width;      /* display area: width and height of display area in micrometres (not millimetres to avoid float) */
  uint32_t  dsparea_height;     /*   optional. if dimensions are unknown values should be left 0 */
  
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
void gd_hw_init(void);
//! update display from buffer
void lh115_update(void);

#ifdef __cplusplus
}
#endif
