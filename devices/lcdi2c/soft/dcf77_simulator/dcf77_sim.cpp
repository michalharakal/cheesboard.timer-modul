// based on http://www.captain.at/rtai-dcf77-simulator.php

#include <stdlib.h>
#include <inttypes.h>
#include <util/delay.h>     
#include <avr/io.h>
#include <avr/pgmspace.h>

#include "../common/uart.h"
#include "dcf77_sim.h"

DCF77Simulator::DCF77Simulator(int pin)
{
  _pin = pin;
  _turbo_mode = 10; // default factor
}

uint8_t DCF77Simulator::get_turbo_mode(void)
{
  return _turbo_mode;
}

void DCF77Simulator::register_uiCallback(uint8_t(*uiCallback)(void))
{
  _uiCallbackFnc = uiCallback;
}

void DCF77Simulator::set_time(uint8_t hour, uint8_t minutes)
{
  _hour = hour;
  _minutes = minutes;   
}
    
void DCF77Simulator::set_date(uint8_t year, uint8_t month, uint8_t day, uint8_t dow)
{
  _year = year;
  _month = month;
  _day = day;
  _dow = dow;
}

void DCF77Simulator::set_turbo_mode(uint8_t turbo_mode)
{
  _turbo_mode = turbo_mode;
}

void DCF77Simulator::get_time(uint8_t *hour, uint8_t *minutes)
{
  *hour = _hour;
  *minutes = _minutes;
 
}

uint8_t DCF77Simulator::get_secs(void)
{
  return _secs;
}
    
void DCF77Simulator::get_date(uint8_t *year, uint8_t *month, uint8_t *day, uint8_t *dow)
{
  *year = _year;
  *month = _month;
  *day = _day;
  *dow = _dow;
}

// nonzero = success
uint8_t DCF77Simulator::sendone(void) {
  uint8_t t;
  
  PORTC |= (1 << _pin);    
  for (t = 1; t <= 20 * _turbo_mode; t++) 
    _delay_ms(1);  //  1 = 200 ms  
  PORTC &= ~(1 << _pin);
  
  // ui callback to make UI responosive
  if (_uiCallbackFnc != NULL)
     if (_uiCallbackFnc() == 0)
	   return 0;

  for (t = 1; t <= 8 * _turbo_mode; t++) 
    _delay_ms(10); 
  _secs++;	
  bits_send++;
  UDR = '1';    
  return 1;	
}

uint8_t DCF77Simulator::sendzero(void) {
  uint8_t t;
  
  PORTC |= (1 << _pin);  
  
  for (t = 1; t <= 10 * _turbo_mode; t++)
    _delay_ms(1);  //  0 = 100 ms
  PORTC &= ~(1 << _pin);
  // ui callback to make UI responosive
  if (_uiCallbackFnc != NULL)
     if (_uiCallbackFnc() == 0)
	   return 0;
  for (t = 1; t <= 9 * _turbo_mode; t++) 
    _delay_ms(10); 
  _secs++;	
  bits_send++;
  UDR = '0';    
  return 1;		
}

// 
void DCF77Simulator::reset(void) 
{
  uint8_t i;
  for (i = 1; i <= 20 * _turbo_mode; i++) 
    _delay_ms(10); 
  _secs = 0;
}   

uint8_t Convert2PacketBCD(uint8_t value) {
  uint8_t result;
  result = value % 10;
  result += (value - result) / 10 * 0x10;
  return result;
}

void DCF77Simulator::simulate(void) {
  uint8_t i, min, hour, day, wday, month, year;
  uint8_t parity_min, parity_hour, parity_date;

  min = Convert2PacketBCD(_minutes);
  hour = Convert2PacketBCD(_hour);
  day = Convert2PacketBCD(_day);
  wday = Convert2PacketBCD(_dow);
  month = Convert2PacketBCD(_month);
  year = Convert2PacketBCD(_year);
  
  _secs = 0;
  
  
  // generate 20 zeros
  for (i = 1; i <= 20; i++ ) {
  	if (sendzero() == 0) 
      return;
  }

  // send one start bit
  if (sendone() == 0) 
    return;
  // minute
  parity_min = 0;
  for (i = 1; i <= 7; i++) {
    if (min & 1) {
      if (sendone() == 0) 
        return;
      parity_min = parity_min ^ 1;
    } 
    else {
	  if (sendzero() == 0) 
        return;
	}
    min = min >> 1;
  }
  // minute_parity
  if (parity_min) { 
    if (sendone() == 0) 
      return;
  } 
  else { 
    if (sendzero() == 0) 
      return;
	
  }
  // hour
  parity_hour = 0;
  for (i = 1; i <= 6; i++) {
    if (hour & 1) {
      if (sendone() == 0) 
        return;
      parity_hour = parity_hour ^ 1;
    } 
    else { 
	  if (sendzero() == 0) 
        return;
	}
    hour = hour >> 1;
  }
  // hour_parity
  if (parity_hour) { 
    if (sendone() == 0) 
      return;
  } 
  else { 
    if (sendzero() == 0) 
      return;
  }
  parity_date = 0;
  // day
  for (i = 1; i <= 6; i++) {
    if (day & 1) {
      if (sendone() == 0) 
        return;
      parity_date = parity_date ^ 1;
    } 
    else {
      if (sendzero() == 0) 
        return;
    } 
    day = day >> 1;
  }
  // weekday
  for (i = 1; i <= 3; i++) {
    if (wday & 1) {
      if (sendone() == 0) 
        return;
      parity_date = parity_date ^ 1;
    } 
    else {
     if (sendzero() == 0) 
        return;
	} 
    wday = wday >> 1;
  }
  // month
  for (i = 1; i <= 5; i++) {
    if (month & 1) {
      if (sendone() == 0) 
         return;
      parity_date = parity_date ^ 1;
    } 
    else {
      if (sendzero() == 0) 
        return;
	}  
    month = month >> 1;
  }
  // year
  for (i = 1; i <= 8; i++) {
    if (year & 1) {
      if (sendone() == 0) 
        return;
      parity_date = parity_date ^ 1;
    } 
    else {
      if (sendzero() == 0) 
        return;
	}	
    year = year >> 1;
  }
  // date_parity
  if (parity_date) { 
    if (sendone() == 0) 
      return;
  } 
  else { 
    if (sendzero() == 0) 
        return;
  }
  // wait a second
  for (i = 0; i < 10*_turbo_mode; i++) { 
    _delay_ms(10);
  }
}


//02:03 27.10.1996
/*
static const uint8_t test[] = {
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,1,1,1,0,0,0,0,0,0,0,1,0,0,0,0,1,1,1,1,0,0,1,1,1,1,0,0,0,0,1,0,1,1,0,1,0,0,1,0
};
*/


void DCF77Simulator::TestMuster(void)
{
/*  uint8_t i;
  for (i=0;i<59;i++)
   if (test[i])
     sendone();
   else
     sendzero();
  // wait a second
  for (i = 0; i < 10 * _turbo_mode ; i++) { 
    _delay_ms(10);
  }

*/
}

void DCF77Simulator::IncMinute(void)
{
  // increment minute
  _minutes++;
  if (_minutes > 59) {
    _minutes = 0;
    _hour++;

    if (_hour > 23) {
      _hour = 0;
//      _day++;
      // etc...
    }
  }
}  

