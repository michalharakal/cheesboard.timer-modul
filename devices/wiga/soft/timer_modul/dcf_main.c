// main.c
/*----------------------------------------------------------------------*/
/*! \file 
--------------------------------------------------------------------------

  	<h3> MAIN.C </h3>
  	Version	1.0<br>
  	Revision	$Revision: 1.4 $<br>
 	Copyright 	Thomas Wedemeyer, Email: avr@thomas-wedemeyer.de<br> 
  	Datum		$Date§<br>

	<h1>DCF-Clock with Atmel AVR ATMega8</h1>  

/*----------------------------------------------------------------------*/

#include <stdlib.h>
#include <stdio.h>
#include <inttypes.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/signal.h>
#include "define.h"
#include "display.h"
#include "dcf.h"

 
/*----------------------------------------------------------------------*/
// Definition der Variablen und Konstanten
/*----------------------------------------------------------------------*/
const uint8_t LED_ROW[9] = { 0x20,							//!< Maske Port C-> Row0
							0x08, 0x10, 0x20, 0x40, 0x80,	//!< Maske Port D-> Row 1 - Row5
							0x01, 0x02, 0x04 };				//!< Maske Port B-> Row 6 - Row8

const uint16_t mask16[] = {	0x0001, 0x0002, 0x0004, 0x0008, 
							0x0010, 0x0020, 0x0040, 0x0080,
							0x0100, 0x0200, 0x0400, 0x0800,
							0x1000, 0x2000, 0x4000, 0x8000 };

volatile uint8_t	tick = 0;	//!< Zähler Sekundentakt

// Display-Speicher
uint8_t	row[9];				//!< Zeilenarray -> Col sind Bitcodiert 
							//!  x | x | x | CL4 | CL3 | CL2 | CL1| CL0

struct bitfield_format	sec_o;		// Sekunden Einstelle
struct bitfield_format 	sec_t;		// Sekunden Zehnerstelle
struct bitfield_format	minute_o;	// Minuten Einstelle
struct bitfield_format 	minute_t;	// Minuten Zehnerstelle
struct bitfield_format	hour_o;		// Stunden Einstelle
struct bitfield_format 	hour_t;		// Stunden Zehnerstelle

struct time_format	time;	//!< Zeitstruktur
struct dcf_format	dcf;	//!< Controllstruktur für DCF

/*----------------------------------------------------------------------*/
//! Funktion für RS232-Ausgabe eines Zeichens
//! @param	c	Zeichen, dass ausgebenen werden soll
//! @return	Immer Null
/*----------------------------------------------------------------------*/
int _writeChar (char c)
{
    while (!(UCSRA & (1<<UDRE))) {} 
        UDR = c;
	return 0; 
}

/*----------------------------------------------------------------------*/
//! Ausgabefunktion für Strings auf die RS232-Schnittstelle
//! @param	string	Zeiger auf den String
//! return 	Immer Null
/*----------------------------------------------------------------------*/
int _writeString (unsigned char *string) 
{ 
     while (*string) 
         _writeChar (*string++); 
	return 0;
} 


 
/*----------------------------------------------------------------------*/
// Hauptroutine
/*----------------------------------------------------------------------*/
int main()
{
	char cText[] = "AVR DCF-Clock V1.0 (c) Thomas Wedemeyer, 2006\r\n www.thomas-wedemeyer.de\r\n";
	uint8_t	old_seconds = 0;

	// Initialisierung IO-Ports
	DDRC = 0x3F;
	PORTC = 0x20;

	DDRB = 0x07;
	PORTB = 0x07;

	DDRD = 0xF8;
	PORTD = 0xF8;


    UCSRB |= (1<<TXEN); 
    UCSRC = (1<<URSEL)|(1<<UCSZ1)|(1<<UCSZ0); 
    UBRRL = (unsigned char) USART_BAUD_SELECT; 
    _writeString(cText); 

	fdevopen(_writeChar, NULL, 0);

	#ifdef DCF_INV
	_writeString("DCF-Signal inverted\r\n\0");
	#else
	_writeString("DCF-signal not inverted\r\n\0");
	#endif




	init_timer1();
	init_timer2();

	init_bitfields();	// Initialisierung der Zeit-Bitfields
	init_dcf();

	sei();

	for(;;)
	{

		if( tick != 0)
		{
			tick--;

			time.seconds++;
			if(time.seconds > 60)
			{
				time.seconds = 1;

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

				time.dcf_set = 0;
			}

					
			//set_sec_o(time);
			set_bitfield((time.seconds%10),	&sec_o);
			set_bitfield((time.seconds/10),	&sec_t);
			set_bitfield((time.minutes%10),	&minute_o);
			set_bitfield((time.minutes/10),	&minute_t);
			set_bitfield((time.hours%10),	&hour_o);
			set_bitfield((time.hours/10),	&hour_t);

			old_seconds = time.seconds;
		}

		check_dcf();

		// Ausgabe aktualisieren, wenn die DCF-Funktion die Seconds geupdatet hat
		if(old_seconds != time.seconds)
		{
			old_seconds = time.seconds;
			set_bitfield((time.seconds%10),	&sec_o);
			set_bitfield((time.seconds/10),	&sec_t);
			set_bitfield((time.minutes%10),	&minute_o);
			set_bitfield((time.minutes/10),	&minute_t);
			set_bitfield((time.hours%10),	&hour_o);
			set_bitfield((time.hours/10),	&hour_t);
		}

	}
}

