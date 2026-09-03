#ifndef BH1750_H
#define BH1750_H


void bh1750_initialization(i2c_inst_t *i2c);
float bh1750_get_lux(i2c_inst_t *i2c);


#endif