#ifndef AHT20_H
#define AHT20_H

#include <stdbool.h>
#include <hardware/i2c.h>

void aht20_initialization(i2c_inst_t *i2c);
bool get_temp_and_humidity(i2c_inst_t *i2c, float *temperature, float *humidity);

#endif