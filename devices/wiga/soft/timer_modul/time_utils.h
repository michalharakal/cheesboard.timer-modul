#ifdef __cplusplus
extern "C" {
#endif


#ifndef _TIME_UTILS_H
#define _TIME_UTILS_H   1

#include "../common/dcf.h"

extern uint8_t IsTimeEvent(time_format *pEvent);
extern uint8_t IsTimeLess(time_format *pEvent);
extern uint8_t IsTimeGreater(uint8_t pin, time_format *pEvent);
extern uint8_t IsInInterval(time_format *pStart, time_format *pEnd);


#endif 

#ifdef __cplusplus
}
#endif