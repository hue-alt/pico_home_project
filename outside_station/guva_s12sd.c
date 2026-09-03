#include <pico/stdlib.h>
#include <hardware/adc.h>
#include "guva_s12sd.h"



void guva12sd_initialize(unsigned int pin){
    adc_init();
    adc_gpio_init(pin);
}


float get_uv_value(unsigned int pin){
    adc_select_input(pin - 26);
    uint16_t raw = adc_read();
    float voltage = (raw/4095.0f) * 3.3f;
    float uv_index = voltage * 10;
    return uv_index;
}
