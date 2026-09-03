#ifndef BMP280_H
#define BMP280_H
#include <hardware\i2c.h>



bool bmp280_initalization(i2c_inst_t *i2c);
bool bmp280_get_temp_and_pressure(i2c_inst_t *i2c, float *temperature, float *pressure);



#endif

