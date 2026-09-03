#include <pico/stdlib.h>
#include <hardware/adc.h>
#include "fc37.h"



void fc37_initialize(unsigned int pin){
    adc_init();
    adc_gpio_init(pin);
    
}

unsigned int get_rain_coefficient(unsigned int pin){
    adc_select_input(pin - 26);
    float raw = adc_read();
    float current_value = ((4095.0f - raw)/4095.0f) * 100.0f;
    return (int)(current_value);
}


