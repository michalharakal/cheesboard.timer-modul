/*! \keyboard.h \brief header file for analog clock. */
//*****************************************************************************
//
// File Name	: 'keyboard.h'
// Title		: 4*4 keyborad and debounced keys
// Date			: 14/10/2007
// Version		: 0.1
//         
// This code is distributed under the GNU Public License
//		which can be found at http://www.gnu.org/licenses/gpl.txt
//
//*****************************************************************************

#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <inttypes.h>

#if defined( __cplusplus )
extern "C" {
#endif

#define KEY_PORT        FUNC_KEYS_PORT
#define KEY_PIN         FUNC_KEYS_PIN
#define KEY0            0
#define KEY1            1
#define KEY2            2
#define KEY3            3
#define KEY4            4
#define KEY5            5
#define KEY6            6
#define KEY7            7

void InitKeyborad(void);
uint16_t ReadKeyboard(void);
void InitFunctionKeys(void);

uint8_t get_key_press( uint8_t key_mask );
void keyboard_clear(void); 

#if defined( __cplusplus )
}
#endif


#endif
