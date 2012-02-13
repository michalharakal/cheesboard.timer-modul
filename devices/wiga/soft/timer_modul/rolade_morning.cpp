
#include <stdio.h>
#include <avr/io.h>
#include <avr/pgmspace.h>

#include "rolade_morning.h"

#include "../common/dcf.h"

#define DEBUG 1

void RoladeMorning::Init(void)
{
#ifdef DEBUG
    printf("%.2d:%.2d:%.2d - ", time.hours, time.minutes, time.seconds);
    printf("Rolade Init pin%d\n\r", _pin);
#endif

    // day, just rolade up
    if (IsBeforeAutomaticStart())
    {
        NewState(rsNight);
        return;
    }

    // automatic
    if (IsInInterval(&_automatic_start, &_automatic_end))
    {
        NewState(rsAutomatic);
        return;
    }

    NewState(rsDay);
}

void RoladeMorning::NewState(MorningRollerStatus new_status)
{
    switch (new_status) {
    case  rsNight:
        Down();
        break;
    case rsAutomatic:
        break;
    case rsDay:
        Up();
        break;
    }

#ifdef DEBUG
    printf("%.2d:%.2d:%.2d - ", time.hours, time.minutes, time.seconds);
    printf("Rolade Status pin%d old%d new%d\r\n", _pin, status, new_status);
#endif
    status = new_status;
}

void RoladeMorning::updateStatus (uint8_t sun_here, uint8_t sun_gone, uint8_t sunset)
{
    switch (status) {
    case  rsNight: // roller shutter up
        break;
    case rsAutomatic:
        if (IsTimeAutomaticEnd())
            NewState(rsDay);
        else
            UpdateRegulatingStatus(sun_here, sun_gone);
        break;
    case rsDay:
        break;
    }
}
