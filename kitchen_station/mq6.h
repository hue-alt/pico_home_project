#ifndef MQ6_H
#define MQ6_H

#include <stdio.h>
#include "pico/stdlib.h"
#include <stdbool.h>

void mq6_initialize(unsigned int pin);
bool is_mq6_alarm(unsigned int pin);

#endif 

