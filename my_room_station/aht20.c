#include <pico/stdlib.h>
#include <hardware/i2c.h>
#include "config.h"
#include "aht20.h"

void aht20_initialization(i2c_inst_t *i2c){
    i2c_init(i2c, 100000);
    gpio_set_function(aht20_bmp280_sda1_pin, GPIO_FUNC_I2C);
    gpio_set_function(aht20_bmp280_scl1_pin, GPIO_FUNC_I2C);
    gpio_pull_up(aht20_bmp280_scl1_pin);
    gpio_pull_up(aht20_bmp280_sda1_pin);
}

bool get_temp_and_humidity(i2c_inst_t *i2c, float *temperature, float *humidity){
    uint8_t command[3] = {0xAC, 0x33, 0x00};
    i2c_write_blocking(i2c, 0x38, command, 3, false);
    sleep_ms(100);
    uint8_t buffer[6];
    int read_bytes = i2c_read_blocking(i2c, 0x38, buffer, 6, false);
    if(read_bytes != 6){
        return false;
    }

    uint32_t raw_humidity = ((uint32_t)buffer[1] << 12) | ((uint32_t)buffer[2] << 4) | ((uint32_t)buffer[3] >> 4);
    uint32_t raw_temperature = (((uint32_t)buffer[3] & 0x0F) << 16) | ((uint32_t)buffer[4] << 8) | (uint32_t)buffer[5];

    *humidity = ((float)raw_humidity / 1048576.0f) * 100.0f;
    *temperature = ((float)raw_temperature / 1048576.0f) * 200.0f - 50.0f;

    return true;
}