#ifndef DATARECEPTION_H
#define DATARECEPTION_H


#include <stdbool.h>
#include <stdint.h>
#include "pico/stdlib.h"
#include "config.h"

typedef enum {
    STATION_OUTSIDE = 1,
    STATION_KITCHEN = 2,
    STATION_ROOM    = 3
} station_number;

typedef struct __attribute__((packed)){
    uint8_t station_id;
    float humidity;
    float temperature;
    float lux_value;
    double temperature_outside;
    float rain_coefficient;
    float uv_index;
    float pressure;
    uint32_t vibration_count;
}outside_data;

typedef struct __attribute__((packed)) {
    uint8_t station_id;
    float decibel_level;
    float carbon_dioxide_level;
    float dust_density;
    bool is_carbon_monoxide;
    bool is_gas_leak;
} kitchen_data;


void data_reception_initialization(void);
bool get_kitchen_data(kitchen_data *data);
bool get_outside_data(outside_data *data);
#endif