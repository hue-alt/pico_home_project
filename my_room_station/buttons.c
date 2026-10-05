#include <pico/stdlib.h>
#include "buttons.h"
#include "config.h"

void buttons_initialization(void){
    gpio_init(button_first_screen_pin);
    gpio_set_dir(button_first_screen_pin, GPIO_IN); 
    gpio_pull_up(button_first_screen_pin);

    gpio_init(button_second_screen_pin);
    gpio_set_dir(button_second_screen_pin, GPIO_IN);
    gpio_pull_up(button_second_screen_pin);
}

uint8_t buttons_logic(void){
    static uint8_t current_screen = 1;
    static bool last_next = true;
    static bool last_previous = true;
    static uint32_t last_press_time = 0;

    bool next = gpio_get(button_first_screen_pin);
    bool previous = gpio_get(button_second_screen_pin);
    uint32_t now = to_ms_since_boot(get_absolute_time());

    if(now - last_press_time >= 150){
        if(next == 0 && last_next == 1){
            current_screen++;
            if(current_screen > 3){
                current_screen = 1;
            }
            last_press_time = now;
        }else if(previous == 0 && last_previous == 1){
            if(current_screen <= 1){
                current_screen = 3;
            }else{
                current_screen--;
            }
            last_press_time = now;
        }
    }

    last_next = next;
    last_previous = previous;

    if(current_screen < 1 || current_screen > 3){
        current_screen = 1;
    }
    return current_screen;
}