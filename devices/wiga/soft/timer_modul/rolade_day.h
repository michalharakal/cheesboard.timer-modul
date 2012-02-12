/**
 * rolade_day.h 
 */

#ifndef RoladeDay_h
#define RoladeDay_h

#include <inttypes.h>

#include "rolade_base.h"


// roller shutter states during day
typedef enum
{
  rsDay,
  rsAutomatic,    
  rsBeforeEvening,
  rsDarkness
} RollerStatus;

class RoladeDay : public RoladeBase
{
  private:
    RollerStatus status;	
	void NewState(RollerStatus new_status);	
	void NewRegulState(RegulatingStatus new__regul_status);	
	uint8_t IsTimeForMorning(void);
  public:
    virtual void Init();
   	void updateStatus (uint8_t sun_here, uint8_t sun_gone, uint8_t sunset);
};

extern void GetWindwoUpTimeFromWeekOfDay(time_format *pEvent);

#endif
