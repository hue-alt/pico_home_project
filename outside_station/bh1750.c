#include <pico/stdlib.h>
#include <hardware/i2c.h>
#include "config.h"
#include "bh1750.h"


void bh1750_initialization(i2c_inst_t *i2c){
    i2c_init(i2c, 100 * 1000);
    gpio_set_function(bh1750_sda0_pin, GPIO_FUNC_I2C);
    gpio_set_function(bh1750_scl0_pin, GPIO_FUNC_I2C);
    gpio_pull_up(bh1750_sda0_pin);
    gpio_pull_up(bh1750_scl0_pin);
    uint8_t command = 0x10; //take measurement
    i2c_write_blocking(i2c, 0x23, &command, 1, false);
}
float bh1750_get_lux(i2c_inst_t *i2c){
    uint8_t buffer[2];
    int read_bytes = i2c_read_blocking(i2c, 0x23, buffer, 2, false);
    if(read_bytes != 2){
        return -1.0f; //error handling
    }else{
        uint8_t high_byte = buffer[0];
        uint8_t low_byte = buffer[1];
        uint16_t raw_value = (high_byte * 256) + low_byte;
        float lux_value = raw_value/1.2f;
        return lux_value;
    }
}