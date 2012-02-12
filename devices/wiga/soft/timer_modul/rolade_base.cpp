/*
  Rolade.cpp - class for rolade states handling
*/
// letzte �nderung am:
// 30.09.2009 
// 17.07.2010, Rolladen hoch SA: von 7:45 --> 7:30
// 19.09.2010, Rolladen hoch 7:00
// 28.09.2010, Rolladen hoch 7:20
// 12.12.2010, Rolladen hoch 7:45
//             SA: von 7:30 --> 7:45
// 07.03.2011, Rolladen hoch 7:00
#include <stdio.h>
#include <avr/io.h>
#include <avr/pgmspace.h>

#include "rolade_base.h"

#include "../common/dcf.h"

#define ROLLADEN_PORT      PORTB

#define DEBUG 1


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



void RoladeBase::Up(void)
{
#ifdef DEBUG
  printf("%.2d:%.2d:%.2d - ", time.hours, time.minutes, time.seconds);
  printf("Up pin%d\r\n", _pin);
#endif   
  ROLLADEN_PORT &= ~(1 << _pin);
}

void RoladeBase::Down(void)
{
#ifdef DEBUG
  printf("%.2d:%.2d:%.2d - ", time.hours, time.minutes, time.seconds);
  printf("Down pin%d\r\n", _pin);
#endif     
  ROLLADEN_PORT |= (1 << _pin);
}


void RoladeBase::setPin(uint8_t pin)
{
  _pin = pin;   
  Up(); 
}

void RoladeBase::setAutomaticInterval(uint8_t h_start, uint8_t m_start, uint8_t h_end, uint8_t m_end)
{
  _automatic_start.hours = h_start;
  _automatic_start.minutes = m_start;
  _automatic_end.hours = h_end;
  _automatic_end.minutes = m_end;
}

void RoladeBase::NewRegulState(RegulatingStatus new__regul_status)
{
  if (regul_status == new__regul_status)
    return;
   switch (new__regul_status) {
     case  regManual: 
	   Up();
	   break;
     case regDown:
	   Down();
	   break;
     case regUp:
	   Up();
	   break;

	}  
#ifdef DEBUG
     printf("%.2d:%.2d:%.2d - ", time.hours, time.minutes, time.seconds);
     printf("Automatic pin%d old%d new%d\r\n", _pin, regul_status, new__regul_status);
#endif   
    regul_status = new__regul_status;
}	


void RoladeBase::UpdateRegulatingStatus(uint8_t sun_here, uint8_t sun_gone)
{
  if (_automatic) {
    // shutter is up and the sun is here
    if ((sun_here) && (sun_gone == 0))
      NewRegulState(regDown);
    
    if ((sun_here == 0) && (sun_gone))
     NewRegulState(regUp);
  }
  else
	NewRegulState(regManual);
}	

void RoladeBase::setAutomaticActive(uint8_t automatic)
{
  // only change
  if (automatic != _automatic) {
#ifdef DEBUG
    printf("%.2d:%.2d:%.2d - ", time.hours, time.minutes, time.seconds);
    printf("Autom mode change old%d new%d\r\n", _automatic, automatic);
#endif   
    
    _automatic = automatic; 
  }
}

uint8_t RoladeBase::IsTimeAutomaticStart(void)
{
  return IsTimeEvent(&_automatic_start);
}

uint8_t RoladeBase::IsTimeAutomaticEnd(void)
{
  return IsTimeEvent(&_automatic_end);
}

uint8_t RoladeBase::IsBeforeAutomaticStart(void)
{
  return IsTimeLess(&_automatic_start);
}

