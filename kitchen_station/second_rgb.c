#include "config.h"
#include <stdbool.h>
#include <pico/stdlib.h>
#include "second_rgb.h"




void second_rgb_initialize(){
    gpio_init(second_diode_red_pin);
    gpio_set_dir(second_diode_red_pin, GPIO_OUT);
    gpio_init(second_diode_blue_pin);
    gpio_set_dir(second_diode_blue_pin, GPIO_OUT);
    gpio_init(second_diode_green_pin);
    gpio_set_dir(second_diode_green_pin, GPIO_OUT);
}

void second_rgb_set_color(bool red, bool green, bool blue){
    gpio_put(second_diode_red_pin, red);
    gpio_put(second_diode_blue_pin, blue);
    gpio_put(second_diode_green_pin, green);
}