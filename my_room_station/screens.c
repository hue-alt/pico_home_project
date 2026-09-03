#include <stdio.h>
#include "pico/stdlib.h"
#include "config.h"
#include "screens.h"
#include "datareception.h"
#include "ssd1306.h"


static ssd1306_t display;


void screens_initialization(i2c_inst_t *i2c){
    i2c_init(i2c, 400000);
    gpio_set_function(outside_and_room_screen_sda_pin, GPIO_FUNC_I2C);
    gpio_set_function(outside_and_room_screen_scl_pin, GPIO_FUNC_I2C);
    gpio_pull_up(outside_and_room_screen_sda_pin);
    gpio_pull_up(outside_and_room_screen_scl_pin);

    ssd1306_init(&display, 128, 64, 0x3C, i2c);
    ssd1306_clear(&display);
    ssd1306_show(&display);
}

static void draw_kitchen_screen(kitchen_data *k_data) {
    char line[32];

    snprintf(line, sizeof(line), "CO2: %.0f ppm", k_data->carbon_dioxide_level); //carbon dioxide level
    ssd1306_draw_string(&display, 0, 0, 1, line);

    snprintf(line, sizeof(line), "Dust: %.1f ug/m3", k_data->dust_density); //dust density
    ssd1306_draw_string(&display, 0, 12, 1, line);

    snprintf(line, sizeof(line), "Noise: %.1f dB", k_data->decibel_level); //noise level(measured in decibel scale)
    ssd1306_draw_string(&display, 0, 24, 1, line);

    if (k_data->is_gas_leak) {
        ssd1306_draw_string(&display, 0, 36, 1, "!!!GAS LEAK!!!"); //gas leak level
    } else {
        ssd1306_draw_string(&display, 0, 36, 1, "No gas detected");
    }

    if (k_data->is_carbon_monoxide) {
        ssd1306_draw_string(&display, 0, 48, 1, "!!!CO DETECTED!!!"); //carbon monoxide detector
    } else {
        ssd1306_draw_string(&display, 0, 48, 1, "No CO detected");
    }
}


static void draw_outside_screen(outside_data *o_data){
    char line[32];
    snprintf(line, sizeof(line), "In:%.1fC  Out:%.1fC", o_data->temperature, o_data->temperature_outside);
    ssd1306_draw_string(&display, 0, 0, 1, line);

    snprintf(line, sizeof(line), "H: %.0f%%  P: %.0f hPa", o_data->humidity, o_data->pressure);
    ssd1306_draw_string(&display, 0, 12, 1, line);

    snprintf(line, sizeof(line), "UV: %.1f  Lux: %.0f", o_data->uv_index, o_data->lux_value);
    ssd1306_draw_string(&display, 0, 24, 1, line);

    snprintf(line, sizeof(line), "Rain coef: %.2f", o_data->rain_coefficient);
    ssd1306_draw_string(&display, 0, 36, 1, line);

    snprintf(line, sizeof(line), "Vibrations: %lu", o_data->vibration_count);
    ssd1306_draw_string(&display, 0, 48, 1, line);
}




static void draw_my_room_screen(room_data *r_data){
    char line[32];
    static const char *days[] = {"ERR", "MN", "TU", "WE", "TH", "FR", "SA", "SD"};
    snprintf(line, sizeof(line), "%s %02d.%02d.%02d %02d:%02d:%02d", //exact date
             days[r_data->time.week_day % 8],
             r_data->time.day,
             r_data->time.month,
             r_data->time.year,
             r_data->time.hours,
             r_data->time.minutes,
             r_data->time.seconds);
    ssd1306_draw_string(&display, 4, 0, 1, line); //exact date
    snprintf(line, sizeof(line), "Temp: %.1fC  Hum: %.0f%%",  r_data->temperature_room, r_data->humidity_room); //temperature and humidity
    ssd1306_draw_string(&display, 0, 16, 1, line);

    snprintf(line, sizeof(line), "Lux: %.0f", r_data->lux_level); //light intensity
    ssd1306_draw_string(&display, 0, 32, 1, line);

    snprintf(line, sizeof(line), "Halas: %.1f dB",  r_data->decibel_level); //noise level
    ssd1306_draw_string(&display, 0, 48, 1, line);
}


void display_screen(uint8_t screen_id, kitchen_data *k_data, outside_data *o_data, room_data *r_data){
    if(k_data == NULL || o_data == NULL || r_data == NULL){
        return;
    }
    ssd1306_clear(&display);
    switch (screen_id)
    {
    case STATION_OUTSIDE:
        draw_outside_screen(o_data);
        break;
    case STATION_KITCHEN:
        draw_kitchen_screen(k_data);
        break;
    case STATION_ROOM:
    default:
        draw_my_room_screen(r_data);
        break;
    }
    ssd1306_show(&display);
}

