/*
  rolade.h
*/

#ifndef Rolade_h
#define Rolade_h

#include <inttypes.h>

#include "../common/dcf.h"


typedef enum
{
    regManual,   // manual modus
    regDown,     // regulating depending on the time
    regUp        //
} RegulatingStatus;

class RoladeBase
{
public:
    void setPin(uint8_t pin);
    void setAutomaticInterval(uint8_t h_start, uint8_t m_start, uint8_t h_end, uint8_t m_end);
    void setAutomaticActive(uint8_t automatic);

    void UpdateRegulatingStatus(uint8_t sun_here, uint8_t sun_gone);
    void NewRegulState(RegulatingStatus new__regul_status);


protected:
    uint8_t _pin;
    uint8_t _automatic;

    time_format _automatic_start;
    time_format _automatic_end;
    RegulatingStatus regul_status;

    uint8_t IsBeforeAutomaticStart(void);
    uint8_t IsTimeAutomaticStart(void);
    uint8_t IsTimeAutomaticEnd(void);

public:
    virtual void Init(void);
    void Up(void);
    void Down(void);
};

extern uint8_t IsTimeEvent(time_format *pEvent);
extern uint8_t IsTimeLess(time_format *pEvent);
extern uint8_t IsTimeGreater(uint8_t pin, time_format *pEvent);
extern uint8_t IsInInterval(time_format *pStart, time_format *pEnd);


#endif
