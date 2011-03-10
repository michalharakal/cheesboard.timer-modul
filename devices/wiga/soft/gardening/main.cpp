#include <stdio.h>
#include "rolade_properties.hpp"
#include "Wire.h"

RoladeProperties rollade[4];

int main (void)
{
    char _bytes[20];
    size_t size;
    Wire.begin();

	rollade[0].SetAutomaticInterval(1,2,3,4);
    rollade[0].Serialize(_bytes, &size);
}
