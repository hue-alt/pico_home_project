#include <pico/stdlib.h>
#include "buttons.h"
#include "config.h"

void buttons_initialization(){
    gpio_init(button_first_screen_pin);
    gpio_set_dir(button_first_screen_pin, GPIO_IN); 
    gpio_pull_up(button_first_screen_pin);

    gpio_init(button_second_screen_pin);
    gpio_set_dir(button_second_screen_pin, GPIO_IN);
    gpio_pull_up(button_second_screen_pin);
}

uint8_t buttons_logic(){
    static uint8_t current_screen = 1;
    bool next = gpio_get(button_first_screen_pin);
    bool previous = gpio_get(button_second_screen_pin);
    if(next == 0){
        current_screen++;
        if(current_screen > 3){
            current_screen = 1;
        }
        sleep_ms(300);
    }
    if(previous == 0){
        current_screen--;
        if(current_screen < 1){
            current_screen = 3;
        }
        sleep_ms(300);
    }
    return current_screen;
}