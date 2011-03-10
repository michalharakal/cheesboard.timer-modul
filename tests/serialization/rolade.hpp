/*
  rolade.h
*/

#ifndef ROLADEDATA_HPP
#define ROLADEDATA_HPP

#include <inttypes.h>


class RoladeData
{
  public:
    void SetPin(uint8_t pin);
	void UpdateStatus (uint8_t sun_here, uint8_t sun_gone, uint8_t sunset);
	void SetAutomaticInterval(uint8_t h_start, uint8_t m_start, uint8_t h_end, uint8_t m_end);
	void SetAutomatik(uint8_t automatic);
	void Init(void);


  private:
    uint8_t _pin;
	uint8_t _automatic;
	uint8_t start_hour;
	uint8_t start_minute;
	uint8_t start_seconds;
	
	uint8_t end_hour;
	uint8_t end_minute;
	uint8_t end_seconds;
};

#endif
