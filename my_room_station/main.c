#include <stdio.h>
#include <config.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "hardware/adc.h"
#include "buttons.h"
#include "aht20.h"
#include "ds3231.h"
#include "max4466.h"
#include "screens.h"
#include "temt6000.h"
#include "datareception.h"



int main(){
    stdio_init_all();
    sleep_ms(2000);
    aht20_initialization(i2c1);
    ds3231_initialization(i2c0);
    max4466_initialize(max4466_pin);
    temt6000_initialization(temt6000_pin);
    buttons_initialization();
    data_reception_initialization();
    screens_initialization(i2c0);


    room_data rdata = {0};
    kitchen_data kdata = {0};
    outside_data odata = {0};


    uint32_t temp = 0;

    while(true){
        get_kitchen_data(&kdata);
        get_outside_data(&odata);
        uint8_t current_screen = buttons_logic();

        uint32_t time = to_ms_since_boot(get_absolute_time());
        if(time - temp >= 1000){
            ds3231_get_time(i2c0, &rdata.time);
            rdata.decibel_level = max4466_decibel_level(max4466_pin);
            rdata.lux_level = temt6000_get_lux(temt6000_pin);
            get_temp_and_humidity(i2c1, &rdata.temperature_room, &rdata.humidity_room);
            temp = time;
        }
        display_screen(current_screen, &kdata, &odata, &rdata);
        sleep_ms(20);
    }

    return 0;
}