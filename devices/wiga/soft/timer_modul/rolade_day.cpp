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
#include <avr/io.h>
#include <avr/pgmspace.h>

#include "rolade_day.h"

#include "../common/dcf.h"

#define ROLLADEN_PORT      PORTB

#define DEBUG 1

void RoladeDay::Init(void)
{
#ifdef DEBUG
  printf("%.2d:%.2d:%.2d - ", time.hours, time.minutes, time.seconds);
  printf("Rolade Init pin%d\n\r", _pin);
#endif   

  // day, just rolade up
  if (IsBeforeAutomaticStart())
  {
    NewState(rsDay);
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

void RoladeDay::NewState(RollerStatus new_status)
{
  switch (new_status) {
  case  rsDay: // roller shutter up
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

uint8_t RoladeDay::IsTimeForMorning(void)
{
  time_format morning_time;
  GetWindwoUpTimeFromWeekOfDay(&morning_time);
  return IsTimeEvent(&morning_time);
}

void RoladeDay::updateStatus (uint8_t sun_here, uint8_t sun_gone, uint8_t sunset)
{
  switch (status) {
  case  rsDay: // roller shutter up
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
	  NewState(rsDay);
    break;
  }
}

void GetWindwoUpTimeFromWeekOfDay(time_format *pEvent)
{
    pEvent->hours = 7; // im Sommer 6:30, 
    pEvent->minutes = 30; // im Winter 7:20 // ab 12.12.2010: 7:45 // ab 07.03.2011: 7:00 // ab 10.11.11 7:30
    switch (dcf.wkday) {
        case 6:  // SA
          pEvent->hours = 7;
          pEvent->minutes = 30;   // war 45 // ab 17.07.10: 30 // ab 12.12.2010: 45
          break;
        case 7:  //SO
          pEvent->hours = 8;
          pEvent->minutes = 00;
          break;            
        }
}


