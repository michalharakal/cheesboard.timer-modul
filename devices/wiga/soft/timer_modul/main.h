#ifdef __cplusplus
extern "C" {
#endif


#ifndef _MAIN_H
#define _MAIN_H   1


//***************************************
// Strukturen Defintion
//***************************************

// Ausgang
typedef struct {
  uint8_t pin;  	
  uint8_t port;	  
}tDigitalOut;

// Motorstatus
typedef struct {
  uint16_t pos;  	
  uint16_t soll_pos;  	
  uint16_t pause_counter;  	
  uint16_t last_manual_pos;
  uint8_t last_direction;  
  uint8_t reference;
  uint8_t running;
}tMotor;

// analog Eingang
typedef struct {
  uint8_t channel;  	    // physikal. Kanal
  uint16_t treshold;	    // Grenzwert 
  uint8_t last_value_high;  // vorheriger Wert (High oder Low, zur Grenzen�bergang erkennen)
  uint8_t wert_highDa;      // =1 wenn Bedingung f�r High erf�hlt (z.B SonneDa)
  uint8_t wert_lowDa;       // =1 wenn Bedingung f�r High erf�hlt (z.B SonneWeg)

  uint16_t treshold_high; // Zeitlimit für HightLimit erfült 
  uint16_t counter_high;  // Zeitz�hler für HightLimit erfült 

  uint16_t treshold_low;  // Zeitlimit für LowLimit erfült 
  uint16_t counter_low;   // Zeitz�hler für LowLimit erfült 
  uint8_t inverted;       // =1 logik umdrehen
   
}tAnalog;

extern uint8_t analog_factor;

#endif 

#ifdef __cplusplus
}
#endif
