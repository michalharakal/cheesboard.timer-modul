// dcf.c
/*----------------------------------------------------------------------*/
/*! \file 
--------------------------------------------------------------------------

  	<h3> DCF.C </h3>
  	Version	1.0<br>
  	Revision	$Revision: 1.3 $<br>
 	Copyright 	Thomas Wedemeyer, Email: avr@thomas-wedemeyer.de<br> 
  	Datum		$Date§<br>

	<h3>Routinen für den Empfang des DCF-Signals und der Auswertung </h3>
	Der DCF-Impuls des Empfängermoduls wird über den externen IRQ0 des
	ATMega detektiert.Die Länge des DCF-Impulses wird über den Timer2 im
	1ms-Raster gemessen.
	Um Störimpulse zu erkennen und zu verwerfen wird die Länge und der 
	zeitliche Abstand zum Vorimpuls überprüft. Bei fehlerhaften Impulsen
	wird die Aufzeichung der Bits zurückgesetzt.
	Die gültigen DCF-Impulse werden sortiert in ein Byte-Array gespeichert.
	Wurde ein DCF-Telegramm	vollständig empfangen werden die Daten 
	ausgewertet und dabei das Parity-Bit ausgewertet.   <br>

----------------------------------------------------------------------*/

#include <inttypes.h>
#include <avr/io.h>
#include <stdio.h>
#include <avr/interrupt.h>
#include "dcf.h"
#include "dcfconf.h"

/*----------------------------------------------------------------------*/
//	Debug Variablen
/*----------------------------------------------------------------------*/
uint16_t	db_tcnt2 = 0;

/*----------------------------------------------------------------------*/
//	Einbinden externe Strukturen
/*----------------------------------------------------------------------*/

dcf_format	dcf;	//!< Controllstruktur für DCF
time_format	time;	//!< Zeitstruktur
uint8_t puls_factor;

/*----------------------------------------------------------------------*/
//! Initialisierung der DCF-Funktion
/*----------------------------------------------------------------------*/
void init_dcf(void)
{
	uint8_t	i;
	puls_factor = 10;  
	

	// DCR-Data-Array löschen
	for(i=0; i < 8; i++)
	{
		dcf.data[i] = 0;
	}

	dcf.sec = 0;
	dcf.bitcnt = 0;
        dcf.debug = 0;
	dcf.tcnt = 0;
	time.dcf_set = 0;
	dcf.okay = 0;
	
	INT_CONTROL = START_EDGE;			
	GICR	|= (1<<INT1);				// Int1 = IRQ Enable;
}


/*----------------------------------------------------------------------*/
//! Interruptservice-Routine IRQ1
//! Einlesen der einzelnen DCF-Bits
/*----------------------------------------------------------------------*/
SIGNAL (SIG_INTERRUPT1)
{
	uint8_t		i;
		
    if (INT_CONTROL == START_EDGE)	
	{   		 
	  //		DCF_DATA_PORT &= ~(1 << DCF_DATA_PIN);
		if(dcf.msec > uint16_t(170 * puls_factor))			// Mehr als 1.7Sek. vergangen!
		{				
			dcf.bitcnt = 0;	// Neusynchronistation
			dcf.sec = 0;
			for(i=0; i < 8; i++)
			{
				dcf.data[i] = 0;
			}

			if(dcf.okay == 1)	// Wenn letzter Datensatz okay war,
			{					// dann stelle Uhrzeit								
				time.seconds = 60;
				time.minutes = dcf.min;
				time.hours = dcf.hour;
				time.dcf_set = 1;				
			}
			else {
			  TCNT0 = 111; 
			}  
		}  

		if(dcf.msec > uint16_t(80 * puls_factor))		// Zeitspanne zwischen den IRQ lang bei mind. 800ms,		
		  INT_CONTROL = END_EDGE;			
		
		dcf.msec = 0;			// Zeitmessung für neues Bit starten

	}
	else									// Fallende Flanke
	{
	  //		DCF_DATA_PORT |= (1 << DCF_DATA_PIN);		
		if((dcf.msec > uint8_t(5 * puls_factor)) & (dcf.msec < uint8_t(13 * puls_factor)))	// Größer 70ms, kleiner 130ms -> 0
		{	
			if(dcf.bitcnt > 58)		// hier geht was schief...
			{				
                           dcf.debug = 1;
			}
			else 
			{
				 
				dcf.bitcnt++;		// Erhöhe nur den Bitzähler...
			}
		}
		else if ((dcf.msec > uint8_t(15 * puls_factor)) & (dcf.msec < uint8_t(23 * puls_factor))) // Zeitspanne für eine 1
		{				
			if(dcf.bitcnt > 58)	// Hier geht was schief..
			{
			  dcf.debug = 2;
			}
			else if(dcf.bitcnt == 58)	// Einsortieren der Daten		
				dcf.data[7] = 0x01;		// Parität Datum speichern
			else if (dcf.bitcnt > 49)	// Jahr
				dcf.data[6] |= ( 1 << (dcf.bitcnt - 50));
			else if (dcf.bitcnt > 44)	// Monat
				dcf.data[5] |= (1 << (dcf.bitcnt - 45));
			else if (dcf.bitcnt > 41)	// Wochentag
				dcf.data[4] |= (1 << (dcf.bitcnt - 42));
			else if (dcf.bitcnt > 35)	// Monatstag
				dcf.data[3] |= (1 << (dcf.bitcnt - 36));
			else if (dcf.bitcnt > 28)	// Stunden
				dcf.data[2] |= (1 << (dcf.bitcnt - 29));
			else if (dcf.bitcnt > 20)	// Monat
				dcf.data[1] |= (1 << (dcf.bitcnt - 21));
			else if (dcf.bitcnt > 14)	// Monat
				dcf.data[0] |= (1 << (dcf.bitcnt - 15));
			dcf.bitcnt++;
		}
		else	// Fehlerhafter Impuls
		{			
                        dcf.debug = 3;
			dcf.bitcnt = 0;	// Neusynchronistation			
			dcf.sec = 0;
			for(i=0; i < 8; i++)
			{
				dcf.data[i] = 0;
			}

			db_tcnt2 = dcf.msec;
			time.dcf_set = 0;				
		}


 
		if(dcf.bitcnt > 58)	// Nachricht vollständig!
		{
			dcf.new_frame = 1;
			
		}

		
		INT_CONTROL = START_EDGE;
		dcf.sec = 0;
	} 
}

/*----------------------------------------------------------------------*/
//! \fn Berechnung der geraden Parität
//! @param	value	Byte über das die Parität berechnet werden soll
//! @return	Parität des Bytes
/*----------------------------------------------------------------------*/
uint8_t	check_parity(uint8_t value)
{
	uint8_t	even_parity = 0;

	while(value)
	{
		even_parity ^= (value & 0x01);
		value >>= 1; 
	};
	return even_parity;

}

/*----------------------------------------------------------------------*/
//! \fn Umwandlung der BCD-Zahl in DezimalZahl
//! @param	value	2-Stellige BCD-Zahl
//!	@return	Dezimalkodierte Zahl
/*----------------------------------------------------------------------*/
uint8_t bcd2dez(uint8_t value)
{
	uint8_t dez;

	dez = ((value & 0xF0)>> 4) * 10;
	dez += (value & 0x0F);

	return dez;
}

/*----------------------------------------------------------------------*/
//! \fn Überprüfung und Auswertung des DCF-Telegramms
/*----------------------------------------------------------------------*/
void check_dcf(void)
{
	uint8_t parity;

	if(dcf.new_frame == 1)
	{
                dcf.debug = 8;
		
		dcf.new_frame = 0;
		dcf.okay = 0;

		parity = dcf.data[1] >> 7;
		
		if(check_parity(dcf.data[1]&0x7F) != parity)
		{
	                dcf.debug = 4;
			return;	// Paritätsfehler!
		}

		dcf.min = bcd2dez(dcf.data[1] & 0x7F);	// Umwandlung Minuten
		
		parity = (dcf.data[2] & 0x40) >> 6;

		if(check_parity(dcf.data[2]&0x3F) != parity)
		{
	
                        dcf.debug = 5;
			return;	// Paritätsfehler!
		}			

		dcf.hour = bcd2dez(dcf.data[2] & 0x3F);	// Umwandlung Minuten

		// Auswertung Parität Datum
		parity = check_parity(dcf.data[3]);		// Berechne Parity Kal-Day
		parity |= (check_parity(dcf.data[4])) << 1;	// Weekday
		parity |= (check_parity(dcf.data[5])) << 2;	// Month
		parity |= (check_parity(dcf.data[6])) << 3;	// Weekday

		parity = check_parity(parity);

		if(dcf.data[7] != parity)
		{		
			
                        dcf.debug = 6;
			return;	// Paritätsfehler!
		}

		dcf.day = bcd2dez(dcf.data[3]);
		dcf.wkday = bcd2dez(dcf.data[4]);
		dcf.month = bcd2dez(dcf.data[5]);
		dcf.year = bcd2dez(dcf.data[6]);

                dcf.debug = 0;
		dcf.okay = 1;	// Fertig entschlüsselt!		
	}

}






