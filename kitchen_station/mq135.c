#include <pico/stdlib.h>
#include <hardware/adc.h>
#include "mq135.h"
#include <math.h>


void mq135_initialize(unsigned int pin){
    //adc_select_input(pin - 26);
    adc_init();
    adc_gpio_init(pin);
}

float mq135_get_voltage(unsigned int pin){
    adc_select_input(pin - 26);
    //adc_select_input(pin);
    uint16_t raw = adc_read();
    float voltage = raw * (3.3f / 4095.0f);
    return voltage;
}

float mq135_get_ppm(unsigned int pin){
    float pin_voltage = mq135_get_voltage(pin);
    float voltage_from_sensor = pin_voltage * 1.5f;
    if(voltage_from_sensor < 0.05f){
        voltage_from_sensor = 0.05f;
    }else if(voltage_from_sensor > 4.95f){
        voltage_from_sensor = 4.95f;
    }
    float sensor_resistance = (5.0f - voltage_from_sensor)/voltage_from_sensor;
    if(sensor_resistance <= 0.0f){
        return 0.0f;
    }
    float resistance_air = sensor_resistance / 5.46f;
    if(resistance_air <= 0.0f){
        return 0.0f;
    }
    float carbon_dioxide_ppm = 110.47f * powf(resistance_air, -2.862f);
    if(isnan(carbon_dioxide_ppm) || isinf(carbon_dioxide_ppm) || carbon_dioxide_ppm < 0.0f){
        return 0.0f;
    }
    return carbon_dioxide_ppm;
}