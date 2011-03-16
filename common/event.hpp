/*
  rolade.h
*/

#ifndef EVENT_HPP
#define EVENT_HPP

#include <stdio.h>
#include <inttypes.h>


extern "C" {
#include "event_struct.h"
};

class CCEvent:private EventStruct
{
  public:
    CCEvent();
    ~CCEvent();
    void SetEvent(uint8_t h_start, uint8_t m_start, uint8_t s_start);
    void Serialize(void *dest, size_t *size);
    void Deserialize(void *src);
    uint8_t getRawDataSize(void);
};

#endif
