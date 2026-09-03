#ifndef MQ135_H
#define MQ135_H 
#include "pico/stdlib.h"
#include <stdbool.h>

void mq135_initialize(unsigned int pin);
float mq135_get_ppm(unsigned int pin);


#endif