#include <stdio.h>
#include <string.h>


#include "event.hpp"

void CCEvent::SetEvent(uint8_t h_start, uint8_t m_start, uint8_t s_start)
{
  m_hour = h_start;
  m_minute = m_start;
  m_seconds = s_start;
}

void CCEvent::Serialize(void *dest, size_t *size)
{
  *size = sizeof(CCEvent);
  memcpy(dest, this, sizeof((CCEvent*)this)); 
}

void CCEvent::Deserialize(void *src) 
{
  memcpy(this, src, sizeof((EventStruct*)this)); 
}

uint8_t CCEvent::getRawDataSize(void)
{
  return sizeof(CCEvent);
}

