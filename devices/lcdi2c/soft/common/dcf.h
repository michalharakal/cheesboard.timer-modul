// dcf.h
/*----------------------------------------------------------------------*/
/*! \file 
--------------------------------------------------------------------------

  	<h3> DCF77.H </h3>
  	Version	1.0
  	Revision	$Revision: 1.3 $
 	Copyright 	Thomas Wedemeyer, Email: avr@thomas-wedemeyer.de 
  	Datum		$Date§

	<h3>Datenstrukturen und Prototypen für DCF-Funktion</h3>

----------------------------------------------------------------------*/

#ifdef __cplusplus
extern "C" {
#endif

#ifndef DCF_H
#define DCF_H

/*----------------------------------------------------------------------*/
// Definition der DCF-Hardware
/*----------------------------------------------------------------------*/
#define DCF_INV				//!< Ausgangssignal DCF-Empfänger invertiert */
//#define DCF_NORMAL			//!< Ausgangssignal DCF-Empfänger nicht invertiert


#define INT_CONTROL MCUCR

#ifdef DCF_INV
#define END_EDGE (1 << ISC11)                    // falling
#define START_EDGE ((1 << ISC11) | (1 << ISC10)) // raising
#else
#define START_EDGE (1 << ISC11)                  // falling
#define END_EDGE ((1 << ISC11) | (1 << ISC10))   // raising
#endif

/*----------------------------------------------------------------------*/
// Strukturen
/*----------------------------------------------------------------------*/

typedef struct	 {
	volatile	uint8_t		data[8];	//!< Datenbits -> in Blöcken in einem Byte gespeichert. 
	volatile	uint8_t		sec;		//!< Sekundenzähler [sec] für DCF-Auswertung abgeleitet von T1
	volatile uint8_t		bitcnt;		//!< Bitzähler für den Empfang
	volatile uint16_t	tcnt;		//!< Zwischenspeicher für Zählerwert Timer 1
	volatile uint16_t	msec;		//!< MSek-Zähler von T2 abgeleitet
	volatile uint8_t		min;		//!< Minutenzähler dekodiert
	volatile uint8_t		hour;		//!< Stundenzähler
	volatile uint8_t		wkday;		//!< Wochentag
	volatile uint8_t		day;		//!< Tag
	volatile uint8_t		month;		//!< Monat
	volatile	uint8_t		year;		//!< Jahr
	volatile 	uint8_t		new_frame;		//!< Flag, das neues Datenframe empfangen...
	volatile	uint8_t		okay;		//!< Daten richtig empfangen und ausgewertet!
       volatile	uint8_t		debug;

} dcf_format;

// Zeitstruktur
typedef struct  {
	volatile uint8_t		seconds;	//!< Sekunden
	volatile uint8_t		minutes;	//!< Minuten
	volatile uint8_t		hours;		//!< Stunden
	volatile uint8_t		dcf_set;	//!< Flag, das DCF Zeit aktualisiert hat	
} time_format;


/*----------------------------------------------------------------------*/
// Prototypen
/*----------------------------------------------------------------------*/
extern void init_dcf(void);
extern void check_dcf(void);

extern uint8_t puls_factor;
extern time_format time;
extern dcf_format	dcf;

#endif

#ifdef __cplusplus
}
#endif
