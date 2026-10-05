#include "ds18b20.h"
#include <hardware/sync.h>
#include <math.h>
#include <pico/stdlib.h>

void DS18B20_intialization(unsigned int pin) {
  gpio_init(pin);
  gpio_set_dir(pin, GPIO_IN);
  gpio_pull_up(pin);
}

bool DS18B20_reset(unsigned int pin) {
  uint32_t ints = save_and_disable_interrupts();
  gpio_put(pin, 0);
  gpio_set_dir(pin, GPIO_OUT);
  sleep_us(480);
  gpio_set_dir(pin, GPIO_IN);
  sleep_us(70);
  bool state = gpio_get(pin);
  sleep_us(410);
  restore_interrupts(ints);
  if (state == 1) {
    return false;
  }
  return true;
}

void DS18B20_write_bit(unsigned int pin, bool bit) {
  uint32_t ints = save_and_disable_interrupts();
  gpio_put(pin, 0);
  gpio_set_dir(pin, GPIO_OUT);
  if (bit == true) {
    sleep_us(3);
    gpio_set_dir(pin, GPIO_IN);
    sleep_us(60);
  } else {
    sleep_us(60);
    gpio_set_dir(pin, GPIO_IN);
    sleep_us(5);
  }
  restore_interrupts(ints);
}

bool DS18B20_read_bit(unsigned int pin) {
  uint32_t ints = save_and_disable_interrupts();
  gpio_put(pin, 0);
  gpio_set_dir(pin, GPIO_OUT);
  sleep_us(2);
  gpio_set_dir(pin, GPIO_IN);
  sleep_us(10);
  bool read = gpio_get(pin);
  sleep_us(50);
  restore_interrupts(ints);
  return read;
}

void DS18B20_write_byte(unsigned int pin, uint8_t byte) {
  for (int i = 0; i < 8; i++) {
    bool temp = (byte >> i) & 1; // taking i-bit from byte variable
    DS18B20_write_bit(pin, temp);
  }
}

uint8_t DS18B20_read_byte(unsigned int pin) {
  uint8_t val = 0;
  for (int i = 0; i < 8; i++) {
    bool actual_bit = DS18B20_read_bit(pin);
    if (actual_bit) {
      val |= (1 << i);
    }
  }
  return val;
}
double DS18B20_get_temperature(unsigned int pin) {
  bool flag = DS18B20_reset(pin);
  if (!flag) {
    return -INFINITY; // error handling
  }
  DS18B20_write_byte(pin, 0xCC);
  DS18B20_write_byte(pin, 0x44);
  sleep_ms(750);
  flag = DS18B20_reset(pin);
  if (!flag) {
    return -INFINITY; // error handling
  }
  DS18B20_write_byte(pin, 0xCC);
  DS18B20_write_byte(pin, 0xBE);
  uint8_t least_byte = DS18B20_read_byte(pin);
  uint8_t most_byte = DS18B20_read_byte(pin);
  DS18B20_reset(pin);

  int16_t raw_temp = (int16_t)((most_byte << 8) | least_byte);
  double actual_temp = (double)raw_temp / 16.0;
  return actual_temp;
}