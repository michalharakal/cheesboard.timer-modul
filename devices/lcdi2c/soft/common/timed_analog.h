#ifndef _TIMED_ANALOG_H
#define _TIMED_ANALOG_H   1

#define tavLow 0
#define tavHigh 1
#define tavUndefined 2

typedef struct {
  uint8_t channel;  	
  uint8_t changed;  	
  uint16_t treshold;	
  uint8_t last_value_low;  
  uint16_t timer_treshold_low;
  uint16_t timer_treshold_high;
  uint16_t timer_counter;
}tTimedAnalog;

extern uint8_t tan_getValueType(tTimedAnalog *channel);
extern uint8_t tan_IsLow(tTimedAnalog *channel) ;
extern uint8_t tan_IsHigh(tTimedAnalog *channel);
extern void tan_Update(uint16_t value, tTimedAnalog * channel);
extern void tan_Setup(uint16_t value, tTimedAnalog * channel);

#endif _TIMED_ANALOG_H
