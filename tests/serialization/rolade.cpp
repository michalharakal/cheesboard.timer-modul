/*
  Rolade.cpp - class for rolade states handling
*/
// letzte Änderung am:
// 30.09.2009 
// 17.07.2010, Rolladen hoch SA: von 7:45 --> 7:30
// 19.09.2010, Rolladen hoch 7:00
// 28.09.2010, Rolladen hoch 7:20
// 12.12.2010, Rolladen hoch 7:45
//             SA: von 7:30 --> 7:45
// 07.03.2011, Rolladen hoch 7:00

#include <stdio.h>

#include "rolade.hpp"


#define DEBUG 1


void GetWindwoUpTimeFromWeekOfDay(time_format *pEvent)
{
    pEvent->hours = 7; // im Sommer 6:30, 
    pEvent->minutes = 00; // im Winter 7:20 // ab 12.12.2010: 7:45 // ab 07.03.2011: 7:00
    switch (dcf.wkday) {
    	case 6:  // SA
			pEvent->hours = 7;
			pEvent->minutes = 30;   // war 45; ab 17.07.10: 30; ab 12.12.2010: 45
			break;
    	case 7:  //SO
			pEvent->hours = 8;
			pEvent->minutes = 00;
			break;            
	}
}


void Rolade::Up(void)
{
#ifdef DEBUG
  printf("%.2d:%.2d:%.2d - ", time.hours, time.minutes, time.seconds);
  printf("Up pin%d\r\n", _pin);
#endif   
  //ROLLADEN_PORT &= ~(1 << _pin);
}

void Rolade::Down(void)
{
#ifdef DEBUG
  printf("%.2d:%.2d:%.2d - ", time.hours, time.minutes, time.seconds);
  printf("Down pin%d\r\n", _pin);
#endif     
  //ROLLADEN_PORT |= (1 << _pin);
}


void Rolade::SetPin(uint8_t pin)
{
  _pin = pin;   
  Up(); 
}

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

void Rolade::SetAutomaticInterval(uint8_t h_start, uint8_t m_start, uint8_t h_end, uint8_t m_end)
{
  _automatic_start.hours = h_start;
  _automatic_start.minutes = m_start;
  _automatic_end.hours = h_end;
  _automatic_end.minutes = m_end;
}

void Rolade::Init(void)
{
#ifdef DEBUG
  printf("%.2d:%.2d:%.2d - ", time.hours, time.minutes, time.seconds);
  printf("Rolade Init pin%d\n\r", _pin);
#endif   
  
  time_format morning_time;
  GetWindwoUpTimeFromWeekOfDay(&morning_time);
  // a night from midnight till morning  
  if (IsTimeLess(&morning_time)) {
    NewState(rsDarkness); 
	return;
  }
	  
  // morning time 	  
  if (IsInInterval(&morning_time, &_automatic_start)) {
	NewState(rsMorning); 
	return;
  }	  

  // automatic
  if (IsInInterval(&_automatic_start, &_automatic_end))
  {
	NewState(rsAutomatic);
	return;
  }  
  NewState(rsBeforeEvening);
}

void Rolade::UpdateRegulatingStatus(uint8_t sun_here, uint8_t sun_gone)
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

void Rolade::SetAutomatik(uint8_t automatic)
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

uint8_t Rolade::IsTimeAutomaticStart(void)
{
  return IsTimeEvent(&_automatic_start);
}

uint8_t Rolade::IsTimeAutomaticEnd(void)
{
  return IsTimeEvent(&_automatic_end);
}

uint8_t Rolade::IsTimeForMorning(void)
{
  time_format morning_time;
  GetWindwoUpTimeFromWeekOfDay(&morning_time);
  return IsTimeEvent(&morning_time);
}

void Rolade::NewState(RollerStatus new_status)
{
  switch (new_status) {
  case  rsMorning: // roller shutter up
    Up();    
    break;
  case rsAutomatic:      
    break;	
  case rsBeforeEvening:
    Up();    
	break; 
  case rsDarkness:
    Down();
	break;
  }
  
#ifdef DEBUG
   printf("%.2d:%.2d:%.2d - ", time.hours, time.minutes, time.seconds);
   printf("Rolade Status pin%d old%d new%d\r\n", _pin, status, new_status);
#endif   
  status = new_status;  
}

void Rolade::NewRegulState(RegulatingStatus new__regul_status)
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

void Rolade::UpdateStatus (uint8_t sun_here, uint8_t sun_gone, uint8_t sunset)
{
  switch (status) {
  case  rsMorning: // roller shutter up
    if (IsTimeAutomaticStart()) {
	  NewState(rsAutomatic);
	  UpdateRegulatingStatus(sun_here, sun_gone);
	}  
    break;
  case rsAutomatic:
    if (IsTimeAutomaticEnd())
	  NewState(rsBeforeEvening);
	else
      UpdateRegulatingStatus(sun_here, sun_gone);
    break;
  case rsBeforeEvening:     
    if (sunset)
	  NewState(rsDarkness);  
    break; 
  case rsDarkness:
    if (IsTimeForMorning())
	  NewState(rsMorning);
    break;
  }
}
