#include <pico/stdlib.h>
#include <hardware/adc.h>
#include "max4466.h"
#include "config.h"
#include <math.h>


void max4466_initialize(unsigned int pin){
    adc_init();
    adc_gpio_init(pin);
}

float max4466_decibel_level(unsigned int pin){
    adc_select_input(pin - 26);
    uint32_t start = time_us_32();
    int max_value = -1;
    int min_value = 4096;
    while(time_us_32() - start < 50000){
        uint16_t raw = adc_read();
        if(raw > max_value){
            max_value = raw;
        }
        if(raw < min_value){
            min_value = raw;
        }
    }
    float voltage = (max_value - min_value) * (3.3f / 4095.f);
    if (voltage < 0.001f) return 0.0f;
    float decibel_level = 20 * log10f(voltage / 0.005f);
    return decibel_level;
}