/*! \file bresenham.c \brief bresenham algoritmus for inetegr line painting. */
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

#if defined( __cplusplus )
extern "C" {
#endif

int8_t dx, dy, s1, s2, swapdir, err, i;

void bresenham_init (uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2) 
{
  if ((dx = x2 - x1) != 0)
  {
	if (dx < 0)
	{
		dx = -dx;
		s1 = -1;
	}
	else
  		s1 = 1;
  }
  else
	s1 = 0;	/* dx = abs(x2-x1), s1 = sign(x2-x1)	*/
	
	if ((dy = y2 - y1) != 0)
	{
				if (dy < 0)
				{
					dy = -dy;
					s2 = -1;
				}
				else
					s2 = 1;
			}
			else
				s2 = 0;	/* dy = abs(y2-y1), s2 = sign(y2-y1)	*/

			if (dy > dx)
			{
				swapdir = dx;	/* use swapdir as temp. var.	*/
				dx = dy;
				dy = swapdir;
				swapdir = 1;
			}
			else
				swapdir = 0;

			i   = dx;		/* Init. of loop cnt	*/
			dy <<=1;
			err = dy - dx;	/* Init. of error term	*/
			dx <<=1;    
}

int8_t bresenham_next (int8_t *MoveX, int8_t *MoveY)
{
		*MoveX = 0;
		*MoveY = 0;
	  
		if (i<0)
			return (-1);	/* Beyond last point! */

		if (err >= 0)
		{
			if (swapdir != 0) 
			{
				*MoveX = s1;		  
			}
			else 
			{
				*MoveY = s2;		  
			}
			err -=  dx;
		}
		if (swapdir != 0) 
		{
			*MoveY = s2;		  
		}
		else 
		{
			*MoveX = s1; 		  
		}
   err +=  dy;

	i--;	/* i==0 indicates "last point reached"	*/
	if (i < 0) 
		return -1;
	else
			return 0;  
}

/*
 * 
 * 
 * 
 * 
 * 
int xCenter=100,yCenter=100,r=50;
	int x=0,y=r;
	int d=3-(2*r);
	glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(1,0,0.5);  
    while(x<=y){

		setPixel(xCenter+x,yCenter+y);
		setPixel(xCenter+y,yCenter+x);
		setPixel(xCenter-x,yCenter+y);
		setPixel(xCenter+y,yCenter-x);
		setPixel(xCenter-x,yCenter-y);
		setPixel(xCenter-y,yCenter-x);
		setPixel(xCenter+x,yCenter-y);
		setPixel(xCenter-y,yCenter+x);

		if (d<0)
			d += (4*x)+6;
		else
		{
			d += (4*(x-y))+10;
			y -= 1;
		}
		x++;

	//
* 
* int8_t bresenham_circle_init(int8_t *MoveX, int8_t *MoveY)
{
y=r;
d= -r;
x2m1= -1;

int8_t bresenham_circle_next (int8_t *MoveX, int8_t *MoveY)
{
y=r;
d= -r;
x2m1= -1;
pixel(0,r);
for(x=1;x<r/sqrt(2);x++)
{  x2m1 += 2;
   d+= x2m1;
   if (d>=0) 
   {  y--;
      d -= (y<<1);  Must do this AFTER y-- 
   }
   pixel(x,y);
}

*/
#if defined( __cplusplus )
}
#endif