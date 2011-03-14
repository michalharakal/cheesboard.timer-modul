#include <stdio.h>
#include "rolade_properties.hpp"

RoladeProperties rollade[4];

int main (void)
{
    char _bytes[20];
    size_t size;

	rollade[0].SetAutomaticInterval(1,2,3,4);
    rollade[0].Serialize(_bytes, &size);
    _bytes[0] = 33;
    _bytes[1] = 34;
    _bytes[2] = 35;
    rollade[0].Deserialize(_bytes);
    
}
