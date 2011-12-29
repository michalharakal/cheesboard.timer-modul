#include <inttypes.h>

#include "time_utils.h"

uint8_t IsTimeEvent(time_format *pEvent)
{
  if ((pEvent->hours) || (pEvent->minutes))
    return ((time.hours * 60 + time.minutes) == (pEvent->hours * 60 + pEvent->minutes));   
  else
	return 0;	
}

uint8_t IsTimeLess(time_format *pEvent)
{
  if ((pEvent->hours) || (pEvent->minutes))
    return ((time.hours * 60 + time.minutes) < (pEvent->hours * 60 + pEvent->minutes));   
  else
	return 1;	
}

uint8_t IsTimeGreater(uint8_t pin, time_format *pEvent)
{
  if ((pEvent->hours) || (pEvent->minutes))
    return ((time.hours * 60 + time.minutes) > (pEvent->hours * 60 + pEvent->minutes));   
  else
	return 1;	
}

uint8_t IsInInterval(time_format *pStart, time_format *pEnd)
{
    return (
      ((time.hours * 60 + time.minutes) >= (pStart->hours * 60 + pStart->minutes)) &&
      ((time.hours * 60 + time.minutes) <= (pEnd->hours * 60 + pEnd->minutes)));  
}
