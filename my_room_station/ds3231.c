#include <pico/stdlib.h>
#include "ds3231.h"
#include "config.h"



void ds3231_initialization(i2c_inst_t *i2c){
    i2c_init(i2c, 100000);
    gpio_set_function(ds3231_sda_pin, GPIO_FUNC_I2C);
    gpio_set_function(ds3231_scl_pin, GPIO_FUNC_I2C);
    gpio_pull_up(ds3231_scl_pin);
    gpio_pull_up(ds3231_sda_pin);
}

uint8_t dec_to_bcd(uint8_t value){
    return ((value / 10) << 4) | (value % 10);
}

uint8_t bcd_to_dec(uint8_t value){
    return ((value >> 4) * 10) + (value & 0x0F);
}


bool ds3231_set_time(i2c_inst_t *i2c, const ds3231_time *time){
    uint8_t buffer[8];
    buffer[0] = 0x00;
    buffer[1] = dec_to_bcd(time->seconds);     
    buffer[2] = dec_to_bcd(time->minutes);      
    buffer[3] = dec_to_bcd(time->hours);         
    buffer[4] = dec_to_bcd(time->week_day);     
    buffer[5] = dec_to_bcd(time->day);          
    buffer[6] = dec_to_bcd(time->month);        
    buffer[7] = dec_to_bcd(time->year%100);

    int flag = i2c_write_blocking(i2c, 0x68, buffer, 8, false);
    if(flag == 8){
        return true;
    }
    return false;
}

bool ds3231_get_time(i2c_inst_t *i2c, ds3231_time *time){
    uint8_t reg = 0x00;
    int flag = i2c_write_blocking(i2c, 0x68, &reg, 1, true);
    if(flag != 1){
        return false; //flag for writing data
    }
    uint8_t buffer[7];
    int flag2 = i2c_read_blocking(i2c, 0x68, buffer, 7, false);
    if(flag2 != 7){
        return false; //flag for reading data
    }
    time->seconds  = bcd_to_dec(buffer[0] & 0x7F); 
    time->minutes  = bcd_to_dec(buffer[1] & 0x7F); 
    time->hours    = bcd_to_dec(buffer[2] & 0x3F); 
    time->week_day = bcd_to_dec(buffer[3] & 0x07);
    time->day      = bcd_to_dec(buffer[4] & 0x3F);
    time->month    = bcd_to_dec(buffer[5] & 0x1F); 
    time->year     = 2000 + bcd_to_dec(buffer[6]); 
    return true;
}
