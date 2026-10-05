#ifndef MQ7_H
#define MQ7_H

#include <stdbool.h>


void mq7_initialize(unsigned int pin);
bool is_mq7_alarm(unsigned int pin);

#endif