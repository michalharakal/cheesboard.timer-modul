/*
  tim_analog.h  time shifted analog input with 
*/

#ifndef TIM_ANALOG_H
#define TIM_ANALOG_H

#include <inttypes.h>

class TimedAnalog
{
  public:
    void AnalogSignal(void);
    void SetChannel(uint8_t channel, uint8_t inverted);    
	void SetTreshold(uint16_t treshold, uint32_t treshold_high, uint32_t treshold_low);	
	
	uint8_t IsSignalHigh (void);	
	uint8_t LowDa(void);
	uint8_t HighDa(void);
	void Clean(void);
  private:
	uint8_t _channel;  	    // physikal. Kanal
    uint16_t _treshold;	    // Grenzwert 
    uint8_t _last_value_high;  // vorheriger Wert (High oder Low, zur Grenzenübergang erkennen)
    uint8_t _wert_highDa;      // =1 wenn Bedingung für High erfühlt (z.B SonneDa)
    uint8_t _wert_lowDa;       // =1 wenn Bedingung für High erfühlt (z.B SonneWeg)

    uint32_t _treshold_high; // Zeitlimit für HightLimit erfühlt 
    uint32_t _counter_high;  // Zeitzähler für HightLimit erfühlt 

    uint32_t _treshold_low;  // Zeitlimit für LowLimit erfühlt 
    uint32_t _counter_low;   // Zeitzähler für LowLimit erfühlt 
    uint8_t _inverted;       // =1 logik umdrehen
	
};

#endif
