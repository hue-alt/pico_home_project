#include "bmp280.h"
#include "config.h"
#include <hardware/i2c.h>
#include <pico/stdlib.h>

typedef struct {
  uint16_t temp1;
  int16_t temp2, temp3;
  uint16_t pressure1;
  int16_t pressure2, pressure3, pressure4, pressure5;
  int16_t pressure6, pressure7, pressure8, pressure9;
} bmp_280_vars;

bmp_280_vars coefficents;

static uint8_t bmp280_addr = 0x76;
static bool bmp280_initialized = false;

static bool test_bmp280_addr(i2c_inst_t *i2c, uint8_t addr) {
  uint8_t id_reg = 0xD0;
  uint8_t chip_id = 0;
  int w = i2c_write_blocking(i2c, addr, &id_reg, 1, true);
  if (w != 1) return false;
  int r = i2c_read_blocking(i2c, addr, &chip_id, 1, false);
  if (r != 1) return false;
  return (chip_id == 0x58 || chip_id == 0x60 || chip_id == 0x56 || chip_id == 0x57);
}

bool bmp280_initalization(i2c_inst_t *i2c) {
  i2c_init(i2c, 100000);
  gpio_set_function(aht20_bmp280_sda1_pin, GPIO_FUNC_I2C);
  gpio_set_function(aht20_bmp280_scl1_pin, GPIO_FUNC_I2C);
  gpio_pull_up(aht20_bmp280_sda1_pin);
  gpio_pull_up(aht20_bmp280_scl1_pin);

  if (test_bmp280_addr(i2c, 0x76)) {
    bmp280_addr = 0x76;
  } else if (test_bmp280_addr(i2c, 0x77)) {
    bmp280_addr = 0x77;
  } else {
    uint8_t wake_test[] = {0xF4, 0x27};
    if (i2c_write_blocking(i2c, 0x76, wake_test, 2, false) >= 0) {
      bmp280_addr = 0x76;
    } else if (i2c_write_blocking(i2c, 0x77, wake_test, 2, false) >= 0) {
      bmp280_addr = 0x77;
    }
  }

  uint8_t wake_sensor[] = {0xF4, 0x27};
  i2c_write_blocking(i2c, bmp280_addr, wake_sensor, 2, false);
  sleep_ms(10);

  uint8_t reg = 0x88;
  uint8_t buffer_register[24];
  i2c_write_blocking(i2c, bmp280_addr, &reg, 1, true);
  int read_bytes = i2c_read_blocking(i2c, bmp280_addr, buffer_register, 24, false);
  if (read_bytes != 24) {
    return false;
  }
  coefficents.temp1 = (buffer_register[1] << 8) | buffer_register[0];
  coefficents.temp2 = (int16_t)((buffer_register[3] << 8) | buffer_register[2]);
  coefficents.temp3 = (int16_t)((buffer_register[5] << 8) | buffer_register[4]);
  coefficents.pressure1 = (buffer_register[7] << 8) | buffer_register[6];
  coefficents.pressure2 =
      (int16_t)((buffer_register[9] << 8) | buffer_register[8]);
  coefficents.pressure3 =
      (int16_t)((buffer_register[11] << 8) | buffer_register[10]);
  coefficents.pressure4 =
      (int16_t)((buffer_register[13] << 8) | buffer_register[12]);
  coefficents.pressure5 =
      (int16_t)((buffer_register[15] << 8) | buffer_register[14]);
  coefficents.pressure6 =
      (int16_t)((buffer_register[17] << 8) | buffer_register[16]);
  coefficents.pressure7 =
      (int16_t)((buffer_register[19] << 8) | buffer_register[18]);
  coefficents.pressure8 =
      (int16_t)((buffer_register[21] << 8) | buffer_register[20]);
  coefficents.pressure9 =
      (int16_t)((buffer_register[23] << 8) | buffer_register[22]);
  bmp280_initialized = true;
  return true;
}

bool bmp280_get_temp_and_pressure(i2c_inst_t *i2c, float *temperature,
                                  float *pressure) {
  if (!bmp280_initialized) {
    if (!bmp280_initalization(i2c)) {
      return false;
    }
  }

  uint8_t read_reg = 0xF7;
  uint8_t buffer[6];
  int w = i2c_write_blocking(i2c, bmp280_addr, &read_reg, 1, true);
  if (w != 1) {
    return false;
  }
  int read_bytes = i2c_read_blocking(i2c, bmp280_addr, buffer, 6, false);
  if (read_bytes != 6) {
    return false;
  }
  uint32_t raw_pressure = ((uint32_t)buffer[0] << 12) |
                          ((uint32_t)buffer[1] << 4) |
                          ((uint32_t)buffer[2] >> 4);
  uint32_t raw_temperature = ((uint32_t)buffer[3] << 12) |
                             ((uint32_t)buffer[4] << 4) |
                             ((uint32_t)buffer[5] >> 4);

  // temperature
  // ############################################################################################################################
  double temp_1 = (((double)raw_temperature / 16384.0f) -
                   ((double)coefficents.temp1 / 1024.0)) *
                  (double)coefficents.temp2;
  double temp_2 = (((double)raw_temperature / 131072.0f -
                    (double)coefficents.temp1 / 8192.0) *
                   ((double)raw_temperature / 131072.0f -
                    (double)coefficents.temp1 / 8192.0)) *
                  (double)coefficents.temp3;
  double t = temp_1 + temp_2;
  *temperature = t / 5120.0;

  // air pressure
  // ################################################################################################################################

  double temp3 = (t / 2.0) - 64000.0;
  double temp4 = temp3 * temp3 * (double)coefficents.pressure6 / 32768.0;
  temp4 = temp4 + temp3 * (double)coefficents.pressure5 * 2.0;
  temp4 = (temp4 / 4.0) + ((double)coefficents.pressure4 * 65536.0);
  temp3 = (((double)coefficents.pressure3 * temp3 * temp3 / 524288.0) +
           ((double)coefficents.pressure2 * temp3)) /
          524288.0;
  temp3 = (1.0 + temp3 / 32768.0) * (double)coefficents.pressure1;
  if (temp3 != 0.0) {
    double p = 1048576.0 - (double)raw_pressure;
    p = (p - (temp4 / 4096.0)) * 6250.0 / temp3;
    temp3 = (double)coefficents.pressure9 * p * p / 2147483648.0;
    temp4 = p * (double)coefficents.pressure8 / 32768.0;
    p = p + (temp3 + temp4 + (double)coefficents.pressure7) / 16.0;

    *pressure = (float)(p / 100.0);
  } else {
    *pressure = 0.0f; // division by zero edge case
  }
  return true;
}
