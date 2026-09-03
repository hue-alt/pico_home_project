#include "mq6.h"
#include "pico/stdlib.h"


bool is_mq6_alarm(unsigned int pin){
    unsigned int state = gpio_get(pin);
    if(state == 0){
        return true; //detected gas
    }else{
        return false; 
    }
}

void mq6_initialize(unsigned int pin){
    gpio_init(pin);
    gpio_set_dir(pin, GPIO_IN);
    gpio_disable_pulls(pin);
}
