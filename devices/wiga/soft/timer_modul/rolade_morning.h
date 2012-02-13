/**
 * rolade_day.h
 */

#ifndef RoladeMorning_h
#define RoladeMorning_h

#include <inttypes.h>

#include "rolade_base.h"


// roller shutter states during day
typedef enum
{
    rsNight,
    rsAutomatic,
    rsDay
} MorningRollerStatus;

class RoladeMorning : public RoladeBase
{
private:
    MorningRollerStatus status;
    void NewState(MorningRollerStatus new_status);
public:
    virtual void Init();
    void updateStatus (uint8_t sun_here, uint8_t sun_gone, uint8_t sunset);
};

#endif
