#include <pico/stdlib.h>
#include <hardware/adc.h>
#include "temt6000.h"



void temt6000_initialization(unsigned int pin){
    adc_init();
    adc_gpio_init(pin);
}

uint16_t temt6000_get_lux(unsigned int pin){
    adc_select_input(pin - 26);
    uint16_t raw_value = adc_read();
    uint16_t lux_value = raw_value * 0.1612;
    return lux_value;
}
