#include <pico/stdlib.h>
#include <hardware/timer.h>
#include "sharp_gp2y.h"
#include "config.h"
#include "hardware/adc.h"

static bool sharp_initialized = false;
static bool led_active_high = true;

static void sharp_detect_polarity(void) {
    adc_select_input(sharp_pin - 26);
    
    gpio_put(sharp_led_pin, 0);
    busy_wait_us(280);
    uint16_t raw0 = adc_read();
    busy_wait_us(40);
    
    gpio_put(sharp_led_pin, 1);
    busy_wait_us(280);
    uint16_t raw1 = adc_read();
    busy_wait_us(40);
    
    if (raw1 > raw0) {
        led_active_high = true;
        gpio_put(sharp_led_pin, 0);
    } else {
        led_active_high = false;
        gpio_put(sharp_led_pin, 1);
    }
    sharp_initialized = true;
}

void sharp_gp2y_initialize(void){
    gpio_init(sharp_led_pin);
    gpio_set_dir(sharp_led_pin, GPIO_OUT);
    gpio_put(sharp_led_pin, 0);
    adc_init();
    adc_gpio_init(sharp_pin);
    sharp_detect_polarity();
}

float dust_density_level(void){
    if (!sharp_initialized) {
        sharp_gp2y_initialize();
    }
    adc_select_input(sharp_pin - 26);
    
    gpio_put(sharp_led_pin, led_active_high ? 1 : 0);
    busy_wait_us(280);
    uint16_t raw = adc_read();
    busy_wait_us(40);
    gpio_put(sharp_led_pin, led_active_high ? 0 : 1);
    busy_wait_us(9680);

    float voltage_out = (raw * 3.3f) / 4095.0f;
    float dust_density = 0.0f;
    if (voltage_out > 0.5f) {
        dust_density = (voltage_out - 0.5f) * 200.0f;
    } else if (voltage_out > 0.08f) {
        dust_density = (voltage_out - 0.08f) * 25.0f;
    } else {
        dust_density = 0.0f;
    }
    return dust_density;
}