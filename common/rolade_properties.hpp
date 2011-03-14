/*
  rolade.h
*/

#ifndef ROLADEDATA_HPP
#define ROLADEDATA_HPP

#include <stdio.h>
#include <inttypes.h>


extern "C" {
#include "rolade_properties.h"
};



class RoladeProperties:private RoladePropertiesData
{
  public:
    RoladeProperties();
    ~RoladeProperties();
	void SetAutomaticInterval(uint8_t h_start, uint8_t m_start, uint8_t h_end, uint8_t m_end);
	void Serialize(void *dest, size_t *size);
    void Deserialize(void *src);
    uint8_t getRawDataSize(void);
  private:
};

#endif
