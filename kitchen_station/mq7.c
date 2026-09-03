#include "mq7.h"
#include <pico/stdlib.h>
#include <stdbool.h>

void mq7_initialize(unsigned int pin){
    gpio_init(pin);
    gpio_set_dir(pin, GPIO_IN);
    gpio_disable_pulls(pin);
}

bool is_mq7_alarm(unsigned int pin){
    unsigned int state = gpio_get(pin);
    if(state == 0){
        return true; //detected carbon monoxide
    }else{
        return false; //no carbon monoxide presence
    }
}