#include "pico/stdlib.h"
#include "config.h"
#include "ds18b20.h"
#include <math.h>


void DS18B20_intialization(unsigned int pin){
    gpio_init(pin);
    gpio_set_dir(pin, GPIO_IN);
}

bool DS18B20_reset(unsigned int pin){
    gpio_put(pin, 0);
    gpio_set_dir(pin, GPIO_OUT);
    sleep_us(480);
    gpio_set_dir(pin, GPIO_IN);
    sleep_us(70);
    bool state = gpio_get(pin);
    sleep_us(410);
    if(state == 1){
        return false;
    }
    return true;
}

void DS18B20_write_bit(unsigned int pin, bool bit){
    gpio_put(pin, 0);
    gpio_set_dir(pin, GPIO_OUT);
    if(bit == true){
        sleep_us(2);
        gpio_set_dir(pin, GPIO_IN);
        sleep_us(60);
    }else{
        sleep_us(60);
        gpio_set_dir(pin, GPIO_IN);
        sleep_us(5);
    }
}

bool DS18B20_read_bit(unsigned int pin){
    gpio_put(pin, 0);
    gpio_set_dir(pin, GPIO_OUT);
    sleep_us(2);
    gpio_set_dir(pin, GPIO_IN);
    sleep_us(10);
    bool read = gpio_get(pin);
    sleep_us(50);
    return read;
}

void DS18B20_write_byte(unsigned int pin, uint8_t byte){
    for(int i = 0; i < 8; i++){
        bool temp = (byte >> i) & 1; //taking i-bit from byte variable
        DS18B20_write_bit(pin, temp);
    }   
}

uint8_t DS18B20_read_byte(unsigned int pin){
    uint8_t val = 0;
    for(int i = 0; i < 8; i++){
        bool actual_bit = DS18B20_read_bit(pin);
        if(actual_bit){
            val |= (1 << i);
        }
    }
    return val;
}
double DS18B20_get_temperature(unsigned int pin){
    bool flag = DS18B20_reset(pin);
    if(!flag){
        return -INFINITY; //error handling
    }
    DS18B20_write_byte(pin, 0xCC);
    DS18B20_write_byte(pin, 0x44);
    sleep_ms(750);
    flag = DS18B20_reset(pin);
    if(!flag){
        return -INFINITY; //error handling
    }
    DS18B20_write_byte(pin, 0xCC);
    DS18B20_write_byte(pin, 0xBE);
    uint8_t least_byte = DS18B20_read_byte(pin);
    uint8_t most_byte = DS18B20_read_byte(pin);
    DS18B20_reset(pin);

    int16_t raw_temp = (most_byte << 8) | least_byte;
    double actual_temp = (double)raw_temp / 16.0;
    return actual_temp;
}