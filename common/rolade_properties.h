//
//  rolade_properties.h
//  serialization
//
//  Created by Michal Harakal on 10.03.11.
//  Copyright 2011 www.cheesboard.com. All rights reserved.
//

#include <inttypes.h>


struct RoladePropertiesData {
	uint8_t m_start_hour;
	uint8_t m_start_minute;
	uint8_t m_start_seconds;
	
	uint8_t m_end_hour;
	uint8_t m_end_minute;
	uint8_t m_end_seconds;
};

uint8_t getRawDataSize(void);

