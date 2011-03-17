/*! \file event_struct.h \brief basic data structure for event */
//*****************************************************************************
//
// File Name	: 'event_struct.h'
// Title		: Event data structure
// Author     : Michal Harakal
//
// This code is distributed under the GNU Public License
//		which can be found at http://www.gnu.org/licenses/gpl.txt
//
//   (c)2011 www.cheesboard.com
//*****************************************************************************

#ifndef EVENT_STRUCT_H
#define EVENT_STRUCT_H


#include <inttypes.h>

#if defined( __cplusplus )
extern "C" {
#endif

struct EventStruct {
	uint8_t m_hour;
	uint8_t m_minute;
	uint8_t m_seconds;
};

void init_event( EventStruct* data );
void fini_event( EventStruct* data );

#endif

#if defined( __cplusplus )
}
#endif
