#include <pico/stdlib.h>
#include "sharp_gp2y.h"
#include "config.h"
#include "hardware/adc.h"


void sharp_gp2y_initialize(){
    gpio_init(sharp_led_pin);
    gpio_set_dir(sharp_led_pin, GPIO_OUT);
    gpio_put(sharp_led_pin, 1);
    adc_init();
    adc_gpio_init(sharp_pin);
}

float dust_density_level(){
    adc_select_input(sharp_pin - 26);
    gpio_put(sharp_led_pin, 0); //turning on the diode
    sleep_us(280);
    uint16_t raw = adc_read();
    sleep_us(40);
    gpio_put(sharp_led_pin, 1); //turning off the diode
    sleep_us(9680);
    float voltage_out = (raw * 3.3f)/4095.0f;
    float dust_density = (voltage_out - 0.7) * 500;
    if(dust_density < 0.0f){
        return 0.0f;
    }
    return dust_density;
}