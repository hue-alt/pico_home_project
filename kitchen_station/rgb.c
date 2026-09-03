#include <pico/stdlib.h>
#include <stdbool.h>
#include "config.h"
#include "rgb.h"


void rgb_initialize(){
    gpio_init(diode_red_pin);
    gpio_set_dir(diode_red_pin, GPIO_OUT);
    gpio_init(diode_green_pin);
    gpio_set_dir(diode_green_pin, GPIO_OUT);
    gpio_init(diode_blue_pin);
    gpio_set_dir(diode_blue_pin, GPIO_OUT);
}

void rgb_set_color(bool red, bool green, bool blue){
    gpio_put(diode_red_pin, red);
    gpio_put(diode_green_pin, green);
    gpio_put(diode_blue_pin, blue);
}
