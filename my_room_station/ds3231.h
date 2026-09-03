#ifndef DS3231_H
#define DS3231_H

#include <pico/stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <hardware/i2c.h>


typedef struct{
    uint8_t seconds;
    uint8_t minutes;
    uint8_t hours;
    uint8_t day;
    uint8_t month;
    uint8_t year;
    uint8_t week_day;
}ds3231_time;


void ds3231_initialization(i2c_inst_t *i2c);
bool ds3231_set_time(i2c_inst_t *i2c, const ds3231_time *time);
bool ds3231_get_time(i2c_inst_t *i2c, ds3231_time *time);


#endif