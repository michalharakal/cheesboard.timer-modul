#include <stdio.h>
#include <string.h>


#include "rolade.h"

RoladeProperties::RoladeProperties()
{
}

RoladeProperties::~RoladeProperties()
{
}


void RoladeProperties::SetAutomaticInterval(uint8_t h_start, uint8_t m_start, uint8_t h_end, uint8_t m_end)
{
	m_start_hour = h_start;
	m_start_minute = m_start;
	m_start_seconds = 0;
	
	m_end_hour = h_end;
	m_end_minute = m_end;
	m_end_seconds = 0;
}

void RoladeProperties::Serialize(void *dest, size_t *size)
{
	*size = sizeof(RoladePropertiesData);
	memcpy(dest, this, sizeof(RoladePropertiesData)); 
}

void RoladeProperties::Deserialize(void *src)
{
	memcpy(this, src, sizeof(RoladePropertiesData)); 
}

