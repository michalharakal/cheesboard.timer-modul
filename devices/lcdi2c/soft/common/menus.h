/*! \file menus.h \brief menus painting */
//*****************************************************************************
//
// File Name	: 'menus.h'
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

#ifdef __cplusplus
extern "C" {
#endif


void RepaintMenu(void);

void AssignMenu(const char *menus[8], const char *menuname);

#ifdef __cplusplus
}
#endif