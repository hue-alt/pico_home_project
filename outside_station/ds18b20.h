#ifndef DS18B20_H
#define DS1820_H


void DS18B20_intialization(unsigned int pin);
bool DS18B20_reset(unsigned int pin);
void DS18B20_write_bit(unsigned int pin, bool bit);
bool DS18B20_read_bit(unsigned int pin);
uint8_t DS18B20_read_byte(unsigned int pin);
void DS18B20_write_byte(unsigned pin, uint8_t byte);
double DS18B20_get_temperature(unsigned int pin);


#endif