#ifndef DATATRANSMISSION_H
#define DATATRANSMISSION_H
#include "pico/stdlib.h"
#include <stdbool.h>
#include <stdint.h>
#include "config.h"

typedef struct __attribute__((packed)){
    uint8_t station_id;
    float humidity;
    float temperature;
    float lux_value;
    double temperature_outside;
    float rain_coefficient;
    float uv_index;
    uint32_t vibration_count;
    float pressure;
}outside_station;

typedef enum {
    STATION_OUTSIDE = 1,
    STATION_KITCHEN = 2,
    STATION_ROOM    = 3
} station_number;

void data_transmission_initialization(void);
bool data_transmission(outside_station *data);

#endif