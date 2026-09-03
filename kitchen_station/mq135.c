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
    float sensor_resistance = (5.0f - voltage_from_sensor)/voltage_from_sensor;
    float resistance_air = sensor_resistance / 5.46f;
    float carbon_dioxide_ppm = 110.47f * powf(resistance_air, -2.862f);
    return carbon_dioxide_ppm;

}