/************************************************************************* 
* Title:    C file for auxiliary functions on cheesboar system
* Author:   Michal Harakal, copy from www.mikrokontroler.ner
* File:     $Id $
* Software: AVR-GCC 3.4.3 / avr-libc 1.2.3
* Target:   any AVR device running TWI
* Usage:    see Doxygen manual
**************************************************************************/

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdlib.h>
#include <inttypes.h>
#include "caux.h"
#include "uart.h"

#define SEKUNDE_WEG_BIT       1   
#define ZEHN_MILISEK_WEG_BIT  2   

// 8-bit timer counter
volatile static WORD timer_sec;
volatile static WORD timer_tenMilisec;
// 16-bit timer count
volatile uint32_t timer1_counter;

// timer status
static BYTE timerStatus;

typedef void(*timerCallback)(void);
typedef void(*uartCallback)(char c);

static timerCallback oneSecFnc = NULL;
static timerCallback tenMilisecFnc = NULL;
static timerCallback mainIdleFnc = NULL;
static timerCallback timer8Fnc = NULL;
static uartCallback userUartFnc = NULL;

/**
 @brief reads all analog channel on PHILIPS I2C AD and returns value of desired channael
 @param  channel number
 @return 8-bit ananlog value of desires channel
 */
uint8_t i2c_ReadAD(uint8_t channel)
{
  /*  uint8_t value;
  uint8_t adwar[4];
  
  i2c_start (0x90) ; // start sequence with device- adress 
  i2c_write (4) ;    // adress 0, 4 single ended, auinc on 
  i2c_stop ();       // end control set 

  i2c_start (0x91);  // start read cycle 
  value = i2c_readAck();// dummy- read, convert 0
  adwar [0]= i2c_readAck ();
  adwar [1]= i2c_readAck ();
  adwar [2]= i2c_readAck ();
  adwar [3]= i2c_readNak (); // last byte 
  adwar [3]= i2c_readNak (); // last byte 
  i2c_stop ();

  if (channel < 4)
    return (adwar[channel]);
  else*/
    return 0; 
}  

#define CHANNELOFFSET 0

/**
 @brief reads all analog channel and returns value of desired channael
 @param  channel number
 @return 10-bit analog value of desires channel
 */
uint16_t ReadAD(uint8_t channel, uint8_t internal)
{
  uint8_t i; 
  uint16_t result = 0;         //Initialisieren wichtig, da lokale Variablen 
                               //nicht automatisch initialisiert werden und 
	 	                               //zuf�llige Werte haben. Sonst kann Quatsch rauskommen 
  ADCSRA = (1<<ADEN) | (1<<ADPS1) | (1<<ADPS0);    // Frequenzvorteiler  
 	                               // setzen auf 8 (1) und ADC aktivieren (1) 
	 	  
  ADMUX = channel;                      // Kanal waehlen 
 //ADMUX |= (1<<REFS1) | (1<<REFS0); // interne Referenzspannung nutzen  
  ADMUX |= (1<<REFS0); // externe Referenzspannung nutzen  
	 	  
  /* nach Aktivieren des ADC wird ein "Dummy-Readout" empfohlen, man liest 
    also einen Wert und verwirft diesen, um den ADC "warmlaufen zu lassen" */ 
  ADCSRA |= (1<<ADSC);              // eine ADC-Wandlung  
  while ( ADCSRA & (1<<ADSC) ) { 
     ;     // auf Abschluss der Konvertierung warten  
  } 
	 	  
  /* Eigentliche Messung - Mittelwert aus 4 aufeinanderfolgenden Wandlungen */ 
  for(i=0;i<4;i++) 
  { 
    ADCSRA |= (1<<ADSC);            // eine Wandlung "single conversion" 
    while ( ADCSRA & (1<<ADSC) ) { 
      ;   // auf Abschluss der Konvertierung warten 
    } 
    result += ADCW;                 // Wandlungsergebnisse aufaddieren 
  } 
  ADCSRA &= ~(1<<ADEN);             // ADC deaktivieren (2) 
	 	  
  result /= 4;                     // Summe durch vier teilen = arithm. Mittelwert 
	 	  
  return result; 
}

/** 
 * timer handler (10ms Takt)
 * 
 * @param SIG_OVERFLOW0 
 * 
 * @return 
 */
SIGNAL (SIG_OVERFLOW0)    
{ 
  // reload timer with initial value
  // 255 - 144
  TCNT0 = 111;	
	 
  if (timer8Fnc != NULL)
    timer8Fnc();
  // 10 milisec counter
  timer_tenMilisec--;
  if (timer_tenMilisec == 0) 
  { 
    timerStatus |= (1 << ZEHN_MILISEK_WEG_BIT);
    timer_tenMilisec = 1;
  }

  timer_sec--;
  if (timer_sec == 0) 
  {
    timerStatus |= (1 << SEKUNDE_WEG_BIT);
    timer_sec = 100;
  }

  
}

/**
 * inits 8-Bit timers to get 10ms overflow
 **/
void Init8Timer(void)
{  
  TIMSK |= (1<<TOIE0); 		// enables the T/C0 overflow interrupt   
  TCNT0 = 111;					// start value of T/C0   
  TCCR0 = (1<<CS02)|(1<<CS00); //8bit counter prescale ck/1024  
  TIFR = (1<<ICF1);			// clear ICF1 /clear pending interrupts
  // vars
  timer_sec = 1;
  timer_tenMilisec = 1;
  timerStatus = 0;
}

/**
 * diese Procedure ist jede 1 Sekunde gerufen
 */ 
void Handle_Takt_1_Sec(void) 
{  
/*  uint8_t tmp_sreg;  // temporaerer Speicher fuer das Statusregister
  tmp_sreg = SREG;   // Statusregister (also auch das I-Flag darin) sichern
  cli();             // Interrupts global deaktivieren
  */

  // execute callback function
  if (oneSecFnc != NULL)
    oneSecFnc();

  // bit löschen
  timerStatus &= ~(1 << SEKUNDE_WEG_BIT);         

/*
  SREG = tmp_sreg;  // Status-Register wieder herstellen 
                    // somit auch das I-Flag auf gesicherten Zustand setzen
*/					
}

/**
 * diese Procedure ist jede 10 Milisekunden gerufen
 */ 
void Handle_Takt_10_MiliSec(void) 
{ 
/*
  uint8_t tmp_sreg;  // temporaerer Speicher fuer das Statusregister

  tmp_sreg = SREG;   // Statusregister (also auch das I-Flag darin) sichern
  cli();             // Interrupts global deaktivieren
  */

  // execute callback function
  if (tenMilisecFnc != NULL)
    tenMilisecFnc();

  // bit löschen
  timerStatus &= ~(1 << ZEHN_MILISEK_WEG_BIT);  
  
  /*
  SREG = tmp_sreg;  // Status-Register wieder herstellen 
                    // somit auch das I-Flag auf gesicherten Zustand setzen
					*/
}

/**
 @brief sets callback function for 1 second timer
 @param  pointer on callback function
 */
void RegisterCallback1Sec(void(*OneSecFnc)(void))
{
  oneSecFnc = OneSecFnc;
  Init8Timer();
}

/**
 @brief sets callback function for 10 milisecond timer
 @param  pointer on callback function
 */
void RegisterCallback10MiliSec(void (*TenMiliSecFnc)(void))
{
  tenMilisecFnc = TenMiliSecFnc;
  Init8Timer();  
}

/**
 @brief sets callback function for 10 milisecond timer
 @param  pointer on callback function
 */
void RegisterCallbackMainIdle(void (*MainIdle)(void))
{
  mainIdleFnc = MainIdle; 
}

/**
 @brief sets callback function for handling of serial port
 @param  pointer on callback function
 */
extern void RegisterUserUart(void (*UartFnc)(char c))
{
	userUartFnc = UartFnc;  
}

void RegisterTimer8(void (*Timer8Fnc)(void))
{
  timer8Fnc = Timer8Fnc;
}

/**
 * uart handlers hanles the first received char from UART a makes 
 * standard processing
 */
void HandleUART(void) 
{
  if (uart_data_ready()) {
    unsigned char c;

    c = uart_get_lastc();
	if ((c == 13) || (c == 10))
      userUartFnc(uart_getc());
  }
}

/**
 @brief main endless cycle executing timer and idle handlers
 */
void ExecuteMain(void)
{  
  while (1) {
    // 1 sec
    if (timerStatus & (1 << SEKUNDE_WEG_BIT))
      Handle_Takt_1_Sec() ;
    // 10 milisec
    if (timerStatus & (1 << ZEHN_MILISEK_WEG_BIT)) 
      Handle_Takt_10_MiliSec();
    // handle UART
    HandleUART();
    // idle
    if (mainIdleFnc != NULL)
      mainIdleFnc();
  }
}
