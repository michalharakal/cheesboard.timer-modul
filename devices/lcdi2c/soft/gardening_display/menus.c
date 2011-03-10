/*! \file menus.c \brief menus painting */
//*****************************************************************************
//
// File Name	: 'menus.c'
// Title		: menus
// Date			: 07/10/2007
// Version		: 0.1
//         
// History      : 07/10/2007 
//
// This code is distributed under the GNU Public License
//		which can be found at http://www.gnu.org/licenses/gpl.txt
//
//*****************************************************************************

#include <inttypes.h>
#include "graphics.h"
#include "widgets.h"

static char *curr_menu[8];
static char *menu_name;

void RepaintMenu(void)
{
	// menu name
	Bar(0,56,127,64);
    ShowStr(4, 55, menu_name, -1);
  
	uint8_t i;
	for (i = 0; i < 4; i++)	
	  if (curr_menu[i][0])	  
        MenuButton(0, i * 14, curr_menu[i], 0);		 
	
	for (i = 0; i < 4; i++)	
	  if (curr_menu[i + 4][0])	  
	    MenuButton(94, i * 14, curr_menu[i + 4], 0);		 
}

void AssignMenu(char *menus[8], char *menuname)
{
	menu_name =  menuname;
	uint8_t i;
  for (i = 0; i < 8; i++)
    curr_menu[i] = menus[i];	    
  
}
