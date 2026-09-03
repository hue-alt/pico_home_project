#ifndef SCREENS_H
#define SCREENS_H

#include <stdint.h>
#include "datareception.h"
#include "config.h"
#include "ds3231.h"

typedef struct room_data{
    uint8_t screen_id;
    float temperature_room;
    float humidity_room;
    float decibel_level;
    float lux_level;
    ds3231_time time;
}room_data;


void screens_initialization(i2c_inst_t *i2c);
void display_screen(uint8_t screen_id, kitchen_data *k_data, outside_data *o_data, room_data *r_data);

#endif