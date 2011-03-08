/*$Id: main.c 397 2008-11-25 19:11:02Z miso $ */
/*$Id: main.c 397 2008-11-25 19:11:02Z miso $ */

// Sonderversion basierend auf Stand vom 27.07.09
// letzte Änderung am:
// 30.09.2009 
// 18.05.2010
// 17.07.2010
// 12.12.2010 Rolladen runter 10min --> 30min nach Abendlicht
// 07.03.2011 Rolladen runter 30min --> 20min nach Abendlicht

// TO DO 
//  - LEDs auf neue Definition anpassen und aktualisieren (zuerst Docs PK)
//  - DCF77 emfpängert auf 50 Miliseconds für "0"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/eeprom.h>
#include <avr/pgmspace.h>
#include <inttypes.h>
#ifndef F_CPU
/* wenn F_CPU nicht bereits vorher definiert (z.B. durch
   Übergabe als Parameter zum Compiler innerhalb des Makefiles) */
#define F_CPU 14745600UL     /* Quarz mit 14.7456 Mhz */
#endif
#include <util/delay.h>
#include <avr/eeprom.h>
// global AVRLIB defines
#include "avrlibdefs.h"
// global AVRLIB types definitions
#include "avrlibtypes.h"


#include "global.h"
#include "main.h"
#include "terminal.h"
#include "keys.h"
#include "rolade.h"
#include "tim_analog.h"
#include "../common/i2cmaster.h"
#include "../common/caux.h"
#include "../status.h"
#include "../common/dcf.h"
#include "../common/dcfconf.h"
#include "../common/uart.h"

#define DEBUG 1

time_format sunset_time, window_time;
volatile uint32_t roller_shutters_delay = 0;
volatile uint16_t power_on_led; 
volatile uint16_t bewaessern_count;
volatile uint8_t current_day; 
uint8_t first_time;

time_format SensorCheckStart, SensorCheckEnd, WindowUpTime, BeweassernZeit;

//***************************************
// Variablen
//***************************************

// light status
typedef enum
{
  stTimeReceive,
  stNormal,
  stCheckSunset,
  stDarkness,
} Status;


// power on status
typedef enum
{
  pwoRestarted,
  pwoConfirmed
} PowerOnStatus;

// watering stat puls
typedef enum
{
  wsNone,
  wsHigh
} WateringOnStatus;



static Status status;
static PowerOnStatus pwo_status;
static WateringOnStatus watering_status;
volatile uint8_t cnt = 0;
volatile uint8_t i2c_counter = 0;
static uint8_t sonnen_automatik = 0;
 uint8_t analog_factor = 1;
static uint8_t last_sonne_da;
static uint8_t last_sonne_weg;
static uint8_t roladen_demaerung=0;


#define analog_factor 1

#define POWER_ON_LED_CYCLES   15 // 15 * 10ms = 150ms, blink takt
#define BEWAESSERN_CYCLES     12000 // 12000 x 10ms = 120s, ist minimum, sonst schwingt der Ausgang

#define INPUTS_COUNT          5
#define AN_INPUTS             5

#define DEMMAERUNG            0
#define SONNE1                1
#define SONNE2                2
#define TEMP                  3
#define REGEN                 4

#define OST                   0
#define SUED                  1
#define WEST                  2
#define NORD                  3


// inputs
TimedAnalog aInput[INPUTS_COUNT];
// LEDs
tDigitalOut dOutput[AN_INPUTS];
// roller shutter objects
Rolade rollade[4];

//***************************************
// hardware 
//***************************************

#define ABEND_LICHT        PD5
#define LICHT_PORT         PORTD
#define BEWAESSERN         PD4
#define BEWAESSERN_PORT    PORTD
#define BEWAESSERN_CTRL_PORT DDRD

#define T_SONNE_MODE       PINB4
   
#define ROLLADEN_OST       PB0
#define ROLLADEN_SUED      PB1   
#define ROLLADEN_WEST      PB2
#define ROLLADEN_NORD      PB3
#define ROLLADEN_PORT      PORTB

// start confirmation
#define RESET_LED_1            PD7
#define RESET_LED_2            PD6
#define RESET_LED_PORT         PORTD
#define RESET_LED_DDR          DDRD

#define RESET_CONFIRM_KEY      PB5 
#define RESET_CONFIRM_KEY_PORT PINB

//***************************************
// Hilffunktionen 
//***************************************


void LichtAn (void)
{
  LICHT_PORT |= (1 << ABEND_LICHT); 
}

void LichtAus (void)
{  
  LICHT_PORT &= ~(1 << ABEND_LICHT);  
}

//***************************************
// i2c communication 
//***************************************

void i2c_send_time_data(void);
void i2c_send_date_data(void);
void i2c_send_dcf_status_data(void);
void i2c_send_abend_sunset_data(void);
void i2c_send_window_delay_data(void);


void i2c_send_time_data(void)
{  
  uint16_t snd_cnt = 0;	
  while (i2c_start(I2C_ABEND_LCD_ADRESS + I2C_WRITE) != 0)  {
  	snd_cnt++;
    if ( snd_cnt > 10) 
      return;
  }  
  
  i2c_write (CMD_SEND_TIME_DATA);
  i2c_write (time.hours); 
  i2c_write (time.minutes);
  i2c_write (time.seconds);      
  i2c_stop ();     
}

void i2c_send_date_data(void)
{
  uint16_t snd_cnt = 0;	
  while (i2c_start(I2C_ABEND_LCD_ADRESS + I2C_WRITE) != 0)  {
  	snd_cnt++;
    if ( snd_cnt > 10) 
      return;
  }  
  i2c_write (CMD_SEND_DATE_DATA);
  i2c_write (dcf.day); 
  i2c_write (dcf.wkday);
  i2c_write (dcf.month);  
  i2c_write (dcf.year);    	    
  i2c_stop ();     
}

void i2c_send_dcf_status_data(void)
{  
  uint16_t snd_cnt = 0;	
  while (i2c_start(I2C_ABEND_LCD_ADRESS + I2C_WRITE) != 0)  {
  	snd_cnt++;
    if ( snd_cnt > 10) 
      return;
  }  
  i2c_write (CMD_SEND_DCF_STATUS_DATA);
  i2c_write (dcf.okay);  
  i2c_write (dcf.bitcnt);  
  i2c_write (dcf.debug);  
  i2c_stop ();     
}

void i2c_send_abend_sunset_data(void)
{  
  uint16_t snd_cnt = 0;
  while (i2c_start(I2C_ABEND_LCD_ADRESS + I2C_WRITE) != 0)  {
  	snd_cnt++;
    if ( snd_cnt > 1000) 
      return;
  }  
  
  i2c_write (CMD_SEND_SUNSET_DATA);
  i2c_write ((uint8_t)status);   
  i2c_write (0);   
  i2c_write (sunset_time.hours); 
  i2c_write (sunset_time.minutes);
  i2c_write (SensorCheckEnd.hours); 
  i2c_write (SensorCheckEnd.minutes);
  i2c_stop ();                  
}

void i2c_send_abend_window_data(void)
{
  uint16_t snd_cnt = 0;
  while (i2c_start(I2C_ABEND_LCD_ADRESS + I2C_WRITE) != 0)  {
  	snd_cnt++;
    if ( snd_cnt > 1000) 
      return;
  }  
  
  i2c_write (CMD_SEND_WINDOW_DATA);
  i2c_write (window_time.hours); 
  i2c_write (window_time.minutes);
  i2c_write (WindowUpTime.hours); 
  i2c_write (WindowUpTime.minutes);
  i2c_stop ();                  
}

  
/*
void i2c_send_window_delay_data(void)
{
  // split delay to 8-bit data
  uint16_t delay_sec;
  delay_sec = 900;  
  uint8_t delay_minutes = delay_sec / 60;
  uint8_t delay_seconds = delay_sec % 60;  

  uint16_t snd_cnt = 0;
  while (i2c_start(I2C_ABEND_LCD_ADRESS + I2C_WRITE) != 0)  {
  	snd_cnt++;
    if ( snd_cnt > 1000) 
      return;
  }  

  i2c_write (CMD_SEND_DELAY_DATA);  
  i2c_write (delay_minutes);                
  i2c_write (delay_seconds);   
  i2c_stop ();                               
}
*/

void i2c_send_watering_data(void)
{
  uint16_t snd_cnt = 0;
  while (i2c_start(I2C_ABEND_LCD_ADRESS + I2C_WRITE) != 0)  {
  	snd_cnt++;
    if ( snd_cnt > 1000) 
      return;
  }  

  i2c_write (CMD_SEND_WATERING_DATA);  
  i2c_write (BeweassernZeit.hours);               
  i2c_write (BeweassernZeit.minutes);   
  i2c_stop ();                               
}

void i2c_send_data(void)
{
  switch(i2c_counter) {
   case 0: 
     i2c_send_time_data();
     break;
   case 1:
     i2c_send_date_data();
     break;
   case 2:
     i2c_send_dcf_status_data();
     break;
     case 3:       
       i2c_send_abend_sunset_data();
       break;      
     case 4:
      i2c_send_watering_data();
       break;
       case 5:
    i2c_send_abend_window_data();
       break;
  }
  i2c_counter++;
  if (i2c_counter == 6)
    i2c_counter = 0; 
}

/**
 * LEDs zeigen sofort ob der Schwellenwert Ã¼berschritten ist
 */
void Update_LEDs (void)
{

  uint8_t i;  
  for (i=0; i < INPUTS_COUNT;i++) {
      if (aInput[i].IsSignalHigh ())
        PORTC &= ~(1 << dOutput[i].pin);
      else
        PORTC |= (1 << dOutput[i].pin);    
  }  
    
  // dcf77 input signal on LED
  if (DCF_DATA_IN_PORT & (1 << DCF_DATA_PIN))     
	DCF_LED_PORT |= (1 << DCF_LED_PIN);   
  else  
    DCF_LED_PORT &= ~(1 << DCF_LED_PIN);
}

uint8_t TimeEventInInterval(time_format *pStart, time_format *pEnd)
{
    return (
      ((time.hours * 60 + time.minutes) >= (pStart->hours * 60 + pStart->minutes)) &&
      ((time.hours * 60 + time.minutes) <= (pEnd->hours * 60 + pEnd->minutes)));  
}

void GetBewaessernZeitFromDate(void)
{
  BeweassernZeit.hours = 6;           // war 21:30  17.07.10
  BeweassernZeit.minutes = 15;
  BeweassernZeit.seconds = 0;  


    switch (dcf.month) {
    case  1:
    BeweassernZeit.hours = 25; // nicht bewaessern
    break;
    case  2:
    BeweassernZeit.hours = 25; // nicht bewaessern
    break;
    case  3:
    BeweassernZeit.hours = 25; // nicht bewaessern
    break;
    case  4:
    BeweassernZeit.hours = 6;       // war 21  17.07.10
    break;
    case 5:
    BeweassernZeit.hours = 6;
    break;
    case 6:
    BeweassernZeit.hours = 6;
    break;
    case 7:
    BeweassernZeit.hours = 6; 
    break;
    case 8:
    BeweassernZeit.hours = 6; 
    break;
    case 9:
    BeweassernZeit.hours = 6; 
    break;
    case 10:
    BeweassernZeit.hours = 25; // nicht bewaessern
    break;
    case 11:
    BeweassernZeit.hours = 25; // nicht bewaessern
    break;
    case 12:
    BeweassernZeit.hours = 25; // nicht bewaessern
    break;      
    default:
      BeweassernZeit.hours = 25; // nicht bewaessern
  }  
}

void GetSensorStartFromDate(void)
{
    switch (dcf.month) {
    case  1:
    SensorCheckStart.hours = 16;
    break;
    case  2:
    SensorCheckStart.hours = 17;
    break;
    case  3:
    SensorCheckStart.hours = 17;
    break;
    case  4:
    SensorCheckStart.hours = 19;
    break;
    case 5:
    SensorCheckStart.hours = 20;
    break;
    case 6:
    SensorCheckStart.hours = 21;
    break;
    case 7:
    SensorCheckStart.hours = 20; // 27.07.2009  von 21 auf 20 geändert
    break;
    case 8:
    SensorCheckStart.hours = 20;
    break;
    case 9:
    SensorCheckStart.hours = 19;
    break;
    case 10:
    SensorCheckStart.hours = 17; // 26.10.2009  von 18 auf 17 geändert
    break;
    case 11:
    SensorCheckStart.hours = 16;
    break;
    case 12:
    SensorCheckStart.hours = 16;
    break;      
    default:
      SensorCheckStart.hours = 16;    
  }  
}

uint8_t ZeitInAbendInterval(void)
{	
  GetSensorStartFromDate(); 
  return TimeEventInInterval(&SensorCheckStart, &SensorCheckEnd);
}

uint8_t ZeitZumBewaessern(void)
{	
  GetBewaessernZeitFromDate(); 
  return TimeEventInInterval(&BeweassernZeit, &BeweassernZeit);
}


uint8_t WindowZeitInAbendInterval(void)
{
  time_format before_midnight, after_midnight;
  after_midnight.hours = 0;
  after_midnight.minutes = 0;
  before_midnight.hours = 23;
  before_midnight.minutes = 59;
  // different winow up time depending of day of week
  GetWindwoUpTimeFromWeekOfDay(&WindowUpTime);  
  // evening
  return (TimeEventInInterval(&sunset_time, &before_midnight) || TimeEventInInterval(&after_midnight, &WindowUpTime));    
}  

void NewPowerOnStatus(PowerOnStatus new_status)
{
  power_on_led = POWER_ON_LED_CYCLES;			 
	  
  switch (new_status) {
    case pwoRestarted:
	  power_on_led = POWER_ON_LED_CYCLES;			 	  
	  break;	  
    case pwoConfirmed:	 // kein blinken nach der Bestätigung 	  
	  break;
	  
  }
#ifdef DEBUG
   printf("%.2d:%.2d:%.2d - ", time.hours, time.minutes, time.seconds);
   printf("Power on old%d new%d\r\n", pwo_status, new_status);
#endif
  pwo_status = new_status;
}

void NewWateringStatus(WateringOnStatus new_status)
{
    
  switch (new_status) {
    case wsNone:
	  bewaessern_count = 0;
      // puls low	  
	  BEWAESSERN_PORT &= ~(1 << BEWAESSERN);
	  break;	  
    case wsHigh:	// puls zum Bewaessern start
	  // puls down
	  bewaessern_count = BEWAESSERN_CYCLES;			 	
	  BEWAESSERN_PORT |= (1 << BEWAESSERN); 
	  break;	  
  }
#ifdef DEBUG
  printf("%.2d:%.2d:%.2d - ", time.hours, time.minutes, time.seconds);
  printf("Watering old%d new%d\r\n",watering_status, new_status);
#endif
  watering_status = new_status;  
}

void NewStatus(Status new_status)
{		  
  switch (new_status) {
  case stNormal:
    roladen_demaerung=0;
    LichtAus ();
	// received valid time so update check time
	GetBewaessernZeitFromDate();
	GetSensorStartFromDate(); 
    GetWindwoUpTimeFromWeekOfDay(&WindowUpTime);  
	current_day = dcf.wkday; 
	// init roller shutter objects
	if (first_time) {
	  first_time = 0;
	  uint8_t i;
	  for (i=0;i < 4; i++) {
            rollade[i].SetAutomatik(sonnen_automatik);
  	    rollade[i].Init();
          }
	}  
    break;
  case stTimeReceive:
    LichtAus ();    
	break;    
  case stCheckSunset:
    // received valid time so update check time
	GetSensorStartFromDate(); 
    GetWindwoUpTimeFromWeekOfDay(&WindowUpTime);  
    break;  
  case stDarkness:    
    // store sunset time
    sunset_time.hours = time.hours;
    sunset_time.minutes = time.minutes;
    sunset_time.seconds = time.seconds;    
    // received valid time so update check time
	GetSensorStartFromDate(); 
    GetWindwoUpTimeFromWeekOfDay(&WindowUpTime);  
    LichtAn ();

	roladen_demaerung=0;
	roller_shutters_delay = 120000; // 60000 x 10ms = 600s// ab 12.12.2010: 180000 // ab 07.03.2011: 120000
    break;  
  }
#ifdef DEBUG
   printf("%.2d:%.2d:%.2d - ", time.hours, time.minutes, time.seconds);
   printf("Status old%d new%d\r\n",status, new_status);
#endif
  status = new_status;
}

/**
 * diese Procedure ist jede 1 Sekunde gerufen
 */ 
void Takt_1_Sec(void) 
{
   time.seconds++;
   // if(time.seconds > 59) // 09.08.2009 Änderung von pk, wie in Changeset 621
   if(time.seconds >= 60)

     {
       time.seconds = 0;
       
       if(time.dcf_set == 0)	// Uhr wurde nicht von DCF gestellt!
	 {
	   time.minutes++;
	   
	   if(time.minutes > 59)
	     {
	       time.hours++;
	       
	       time.minutes = 0;
	       if(time.hours >= 24)
		 {
		   time.hours = 0;
		 }
	     }
	 }
       
       //time.dcf_set = 0;
     } 	 
	 if (current_day != dcf.wkday) {
	   current_day = dcf.wkday;
	   GetSensorStartFromDate(); 
       GetWindwoUpTimeFromWeekOfDay(&WindowUpTime);  
     } 
}

void TripleLed(int automatik, int blinken)
{
  if (automatik) {
    if (blinken) {
      RESET_LED_PORT &= ~(1 << RESET_LED_1);
      RESET_LED_PORT ^= (1 << RESET_LED_2);
    }
    else {
      RESET_LED_PORT &= ~(1 << RESET_LED_1);
      RESET_LED_PORT &= ~(1 << RESET_LED_2);
    }
  }
  else {
    if (blinken) {
      RESET_LED_PORT |= (1 << RESET_LED_2);
      RESET_LED_PORT ^= (1 << RESET_LED_1);
    }
    else {
      RESET_LED_PORT |= (1 << RESET_LED_2);
      RESET_LED_PORT |= (1 << RESET_LED_1);
    }
  }
}

/**
 * diese Procedure ist jede 10 Milisekunden gerufen
 */
void Takt_10_MiliSec (void)
{	
  uint8_t last_sonnen_automatik;
  last_sonnen_automatik = sonnen_automatik;

  // automatic switch
  sonnen_automatik = ((PINB & (1 << T_SONNE_MODE)) == 0);  

  dcf.msec += 10;				// Zeitgeber für DCF-Empfang weiterdrehen	
  if (dcf.msec > 5000) {
    // signal ausgefahlen
    time.dcf_set = 0;
  }
  
  // power on status LED
  if (pwo_status == pwoRestarted) {
    power_on_led--;      
	if (power_on_led == 0) {  
          // blink LED
          TripleLed(sonnen_automatik, 1);
          power_on_led = POWER_ON_LED_CYCLES;	
	}  
  }
  else
    TripleLed(sonnen_automatik, 0);

  // update analog signals
  uint8_t i;
  for (i=0; i<AN_INPUTS; i++)
    aInput[i].AnalogSignal();

	
  check_dcf(); 
  
  
  switch (watering_status) {
    case wsNone:
	  if (ZeitZumBewaessern())
	    NewWateringStatus(wsHigh);	  
	  break;
    case wsHigh:
	  bewaessern_count--;
	  if (bewaessern_count == 0)
	    NewWateringStatus(wsNone);	  	  
	  break;
  }
  

  Update_LEDs ();    
  switch (status) {
  case stNormal:    
    if (ZeitInAbendInterval()) 
      NewStatus (stCheckSunset);    
    break;	    
  case stTimeReceive:    
    if (time.dcf_set != 0) {   // gültige Zeit empfangen
      NewStatus (stNormal);	  
	}  
    break;	    
  case stCheckSunset:
    // Sonne weg 
    if (aInput[DEMMAERUNG].LowDa()) {
      NewStatus (stDarkness);      
    }  
    break;
  case stDarkness:    
    if (ZeitInAbendInterval() == 0) 
    {
      NewStatus (stNormal);            
    }  	
    break;  
  }  
  
  if (roller_shutters_delay > 0) {
	roller_shutters_delay --;
	if (roller_shutters_delay == 0) {
	   roladen_demaerung=1;
	   window_time.hours = dcf.hour;
       window_time.minutes = dcf.min;
	}	 	  
  }
  
  // power on status - quick LED blinking if restarted, no after confirmation key
  switch (pwo_status) {  
    case pwoRestarted: // confirm key checked only after start 
	  if (get_key_press( 1 << RESET_CONFIRM_KEY )) {	  	 
	    NewPowerOnStatus(pwoConfirmed);
      }
	  break;	  
    case pwoConfirmed:
	  break;
  };  
  
  // roller shutter works only with valid time
  if (status != stTimeReceive) {  
    uint8_t sonne_da = ((aInput[SONNE1].HighDa()) ||(aInput[SONNE2].HighDa()));
    sonne_da = (sonne_da && aInput[TEMP].HighDa());
    uint8_t sonne_weg = (aInput[SONNE1].LowDa()) &&(aInput[SONNE2].LowDa());

    if (last_sonne_da != sonne_da)
    {
	
	#ifdef DEBUG
      printf("%.2d:%.2d:%.2d - ", time.hours, time.minutes, time.seconds);
      printf("sonne da new%d\r\n", sonne_da);  
#endif
     last_sonne_da = sonne_da;
    } 

    if (last_sonne_weg != sonne_weg)
    {
	#ifdef DEBUG
	 printf("%.2d:%.2d:%.2d - ", time.hours, time.minutes, time.seconds);
       printf("sonne weg new%d\r\n", sonne_weg);  
#endif
      
      last_sonne_weg = sonne_weg;
    } 
  
    // rolades
    for (i=0; i<4; i++) {
        if (last_sonnen_automatik != sonnen_automatik) 
          rollade[i].SetAutomatik(sonnen_automatik);
      rollade[i].UpdateStatus(sonne_da, sonne_weg, roladen_demaerung);
    }
  }  
  // i2c data send only every 150 ms
  cnt++;
  if (cnt > 15   ) {
    cnt = 0; 
    i2c_send_data();  	  	  
  }   

}

void MainIdle (void)
{
  ;				// EMPTY  
}

void InitVariables (void)
{
  first_time = 1;

  // analog inputs
  aInput[SONNE1].SetChannel(7, 0);  
  aInput[SONNE1].SetTreshold(600, 240000, 60000); // Schwelle, Zeit_Signal_low, Zeit_signal_high in 10ms Cyclen
  

  aInput[SONNE2].SetChannel(6, 0);   
  aInput[SONNE2].SetTreshold(600, 240000, 60000);
  
  aInput[TEMP].SetChannel(4, 0);     
  aInput[TEMP].SetTreshold(675, 60000, 60000);  // 22 Grad  Temp = AnalogWert/ 30.69 
  
  aInput[DEMMAERUNG].SetChannel(2, 0);     
  aInput[DEMMAERUNG].SetTreshold(500, 60000, 60000);  // war 600 bis 17.07.10 / war 400 bis 28.09.10
    
  aInput[REGEN].SetChannel(3, 1);     
  aInput[REGEN].SetTreshold(500, 0, 0);
  


  // LEDs
  dOutput[SONNE1].port = PORTC;
  dOutput[SONNE1].pin = 7;
  
  dOutput[SONNE2].port = PORTC;
  dOutput[SONNE2].pin = 6;  
  
  dOutput[TEMP].port = PORTC;
  dOutput[TEMP].pin = 4;    
  
  dOutput[DEMMAERUNG].port = PORTC;
  dOutput[DEMMAERUNG].pin = 2;
  
  dOutput[REGEN].port = PORTC;
  dOutput[REGEN].pin = 3;  
  

  uint8_t i;
  
  for (i = 0; i< AN_INPUTS; i++) {    
    DDRC |= (1 << dOutput[i].pin); // Ausgang		    	     
  }  


  SensorCheckStart.hours = 16;
  SensorCheckStart.seconds = 0;
  SensorCheckStart.minutes = 0;
  
  SensorCheckEnd.hours = 23;
  SensorCheckEnd.minutes = 30;
  SensorCheckEnd.seconds = 0;  
  
  WindowUpTime.hours = 6;    // Zeit in rolade.cpp ändern, hier keine Funktion
  WindowUpTime.minutes = 30; // Zeit in rolade.cpp ändern, hier keine Funktion
  WindowUpTime.seconds = 0;  
  
  BeweassernZeit.hours = 6;     // war 21
  BeweassernZeit.minutes = 15;  // war 30
  BeweassernZeit.seconds = 0;  
  
  
      
  // zuerst Zeit empfangen
  NewStatus(stTimeReceive);
  // powered on
  NewPowerOnStatus(pwoRestarted);
  // baewesserung starten um 21:30
  NewWateringStatus(wsNone);
  
  init_dcf();  
  
  
  rollade[OST].SetPin(ROLLADEN_OST);
  rollade[OST].SetAutomaticInterval(8, 00, 14, 00);	// Zeiten Rolläden unten: HH,MM,HH,MM
  
  
  rollade[SUED].SetPin(ROLLADEN_SUED);
  rollade[SUED].SetAutomaticInterval(15, 00, 15, 30);	
  
  rollade[WEST].SetPin(ROLLADEN_WEST);
  rollade[WEST].SetAutomaticInterval(16, 0, 20, 00);	// im Sommer 16...20,  war 14...15
  
  rollade[NORD].SetPin(ROLLADEN_NORD);
  rollade[NORD].SetAutomaticInterval(19, 30, 20, 00);	// im Sommer 19:30...20. gesetzt, damit Rolläden abends runter fahren können, war 14:30-15   
}

/**
 * init IO ports
 */
void InitIO (void)
{
  // LEDS outputs
  uint8_t i;
  for (i = 0; i< AN_INPUTS; i++)     
    DDRC |= (1 << dOutput[i].pin); // Ausgang		    	     

  // schalter Sonnenautomatik
  DDRB &= ~(1 << T_SONNE_MODE); 

  // relays 
  DDRD |= (1 << ABEND_LICHT);  
  DDRB |= (1 << ROLLADEN_OST) | (1 << ROLLADEN_SUED) | (1 << ROLLADEN_WEST) | (1 << ROLLADEN_NORD);
  // DCF-77 input 
  DCF_DDR &= ~(1 << DCF_DATA_PIN);  // kein pullup ?????
  DCF_PORT |= (1 << DCF_DATA_PIN);  // pullup !!!   
  // DCF-77 data LED 
  DCF_LED_DDR |= (1 << DCF_LED_PIN);  
  
  
  // reset confirmation IOs
  RESET_LED_DDR |= ((1 << RESET_LED_1) | (1 << RESET_LED_2));
  DDRB &= ~ (1 << RESET_CONFIRM_KEY);
  PORTB |= (1 << RESET_CONFIRM_KEY);//pullup  
  
  BEWAESSERN_CTRL_PORT |= ( 1 << BEWAESSERN);
  
}

int main (void)
{
  // uart 
  //uart_init( 47 );  // 19200@14.7456  
  uart_init( 7 );  // 115200@14.7456  
  // init       
  InitIO ();
  i2c_init();
  InitVariables ();  
  // Timer Funktionen registrien
  RegisterCallback1Sec(Takt_1_Sec);
  RegisterCallback10MiliSec (Takt_10_MiliSec);
  RegisterTimer8(KeyDebounceTimerHandler);
  RegisterUserUart(Terminal);
  // enable interrupts
  sei ();
#ifdef DEBUG
	 printf("Timer modul starting ...\n\r");;
#endif 
  // execute main loop
  ExecuteMain();      
}


