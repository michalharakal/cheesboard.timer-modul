/*! \file clock.c \brief bresenham algoritmus for inetegr line painting. */
//*****************************************************************************
//
// File Name	: 'clock.c'
// Title		: analog clock
// Date			: 29/09/2007
// Version		: 0.1
//         
// History      : 12/10/2007 bar
//
// This code is distributed under the GNU Public License
//		which can be found at http://www.gnu.org/licenses/gpl.txt
//
//*****************************************************************************

#include <inttypes.h>
#include <math.h>
#include "graphics.h"

#define 	RAD2DEG(x)   ((float) (M_PI/180.0)*(x))




void paint_marks(uint8_t middle_x, uint8_t middle_y, uint8_t radius, int8_t color)
{
	uint8_t i;
	  
	// seconds  
	for (i = 0; i < 60; i++)	{
		//gd_hw_set_pixel(middle_x + xpos[i], middle_y + ypos[i],  color);	  
		gd_hw_set_pixel(middle_x + radius * cos(RAD2DEG(i * 6)),  middle_y + radius * sin(RAD2DEG(i * 6)), color) ;
	}	
	
	for (i = 0; i < 12; i++)	{
			// hours
		gLine(middle_x + radius * cos(RAD2DEG(i * 30)),  middle_y + radius * sin(RAD2DEG(i * 30)),  
		      middle_x + (radius - 4) * cos(RAD2DEG(i * 30)),  middle_y + (radius - 4) * sin(RAD2DEG(i * 30)),  color);	  
	  	
    }	  
}

void paint_clock(uint8_t middle_x, uint8_t middle_y, uint8_t radius, uint8_t hours, uint8_t minutes, uint8_t secs, int8_t color) 
{
	paint_marks(middle_x, middle_y, radius, color);
	
	int16_t angle_hours;
	
	
	if (minutes > 0)  {
	  angle_hours = (((hours + 1) % 12) * 30);	
	  angle_hours -=  ((60 - minutes) / 2);
	  angle_hours = 90 - angle_hours;
	}
	else {
	  angle_hours = (((hours) % 12) * 30);		 	   
	  angle_hours = 90 - angle_hours;
	}  
	  
	// hand - hours
	gLine(middle_x + (radius / 2) * cos(RAD2DEG(angle_hours)),  
	      middle_y + (radius / 2) * sin(RAD2DEG(angle_hours)),  
		  middle_x,
		  middle_y,      
		  color);	  
	// hand - minutes	  
	gLine(middle_x + (radius - 5) * cos(RAD2DEG(90 - (minutes * 6))),  
	      middle_y + (radius - 5) * sin(RAD2DEG(90 - (minutes * 6))),  
		  middle_x,
		  middle_y,      
		  color);	  
	// hand - sec
	gLine(middle_x + (radius) * cos(RAD2DEG(90 - (secs * 6))),  
	      middle_y + (radius) * sin(RAD2DEG(90 - (secs * 6))),  
		  middle_x,
		  middle_y,      
		  color);	  	  
		  
}
