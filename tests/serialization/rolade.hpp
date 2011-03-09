/*
  rolade.h
*/

#ifndef Rolade_h
#define Rolade_h

#include <inttypes.h>
#include "../common/dcf.h"

// roller shutter states
typedef enum
{
  rsMorning,
  rsAutomatic,
  rsBeforeEvening,
  rsDarkness
} RollerStatus;

typedef enum
{
  regManual,   // manual modus
  regDown,     // regulating depending on the time
  regUp        // in regulating time, but manual modus
} RegulatingStatus;

class Rolade
{
  public:
    void SetPin(uint8_t pin);
	void UpdateStatus (uint8_t sun_here, uint8_t sun_gone, uint8_t sunset);
	void SetAutomaticInterval(uint8_t h_start, uint8_t m_start, uint8_t h_end, uint8_t m_end);
	void SetAutomatik(uint8_t automatic);
	void Init(void);


  private:
    uint8_t _pin;
	uint8_t _automatic;
	time_format _automatic_start;
	time_format _automatic_end;
    RollerStatus status;
	RegulatingStatus regul_status;

	void NewState(RollerStatus new_status);
	void NewRegulState(RegulatingStatus new__regul_status);
    void Up(void);
    void Down(void);

	void UpdateRegulatingStatus(uint8_t sun_here, uint8_t sun_gone);
	uint8_t IsTimeForMorning(void);
    uint8_t IsTimeAutomaticStart(void);
    uint8_t IsTimeAutomaticEnd(void);
};

extern void GetWindwoUpTimeFromWeekOfDay(time_format *pEvent);

#endif
