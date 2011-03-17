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

#include <inttypes.h>
#include "event_struct.h"

#if defined( __cplusplus )
extern "C" {
#endif

// init data sructure with basci data
void init_event( EventStruct* data )
{
  data->m_hour = 0;
  data->m_minute = 0;  
  data->m_seconds = 0;  
}

void fini_event( EventStruct* data )
{
  // empty
}


#if defined( __cplusplus )
}
#endif