#ifndef MAX4466_H
#define MAX4466_H
#include "pico/stdlib.h"


void max4466_initialize(unsigned int pin);
float max4466_decibel_level(unsigned int pin);

#endif