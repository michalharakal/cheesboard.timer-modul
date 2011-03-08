
#include <stdio.h>
#include <inttypes.h>
#include "timed_analog.h"

/**
 @brief returns current analog value type
 @param  channel timed analog channel data
 @return results value type
 */
uint8_t tan_getValueType(tTimedAnalog *channel)
{
  if (channel->last_value_low) 
    return tavLow;
  else
    return tavHigh;
}

/**
 @brief returns true if last analog value is in low range for the defined time
 @param  channel timed analog channel data
 @return nonzero=Low
 */
uint8_t tan_IsLow(tTimedAnalog *channel) 
{
  return (tan_getValueType(channel) == tavLow);
}

/**
 @brief compares analog value with timed analog channel
 @param  channel timed analog channel data
 @return nonzero=High
 */
uint8_t tan_IsHigh(tTimedAnalog *channel)
{
  return (tan_getValueType(channel) == tavHigh);
}

/**
 @brief compares analog value with timed analog channel
 @param  value new analog value
 @param  channel timed analog channel data
 */
void tan_Update(uint16_t value, tTimedAnalog * channel)
{
  uint8_t last;

  last = channel->last_value_low;
 // printf("value %d tresh %d \n\r",value, channel->treshold);
  // if threshold changing
  if ((value >= channel->treshold) ^ channel->last_value_low) {
    // clear counter
    channel->timer_counter = 0;    
  }
  else {
    // different counter values for
    if (channel->last_value_low) {    
      if (channel->timer_counter >= channel->timer_treshold_low)
        channel->last_value_low = (value < channel->treshold);
      else
        channel->timer_counter++;    
    }
    else {
      if (channel->timer_counter >= channel->timer_treshold_high)
        channel->last_value_low = (value < channel->treshold);
      else
        channel->timer_counter++;    
    } 	
  }
  if (last != channel->last_value_low)
    printf("changing %d \n\r", channel->channel);

}

/**
 @brief inits channel without timers
 @param  value current
 @param  channel timed analog channel data
 */
void tan_Setup(uint16_t value, tTimedAnalog * channel)
{
  channel->last_value_low = (value < channel->treshold);
  channel->timer_counter = 0;      
}

