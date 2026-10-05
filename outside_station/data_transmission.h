#ifndef DATATRANSMISSION_H
#define DATATRANSMISSION_H
#include <stdbool.h>
#include <stdint.h>


typedef struct __attribute__((packed)) {
  uint8_t station_id;
  float humidity;
  float temperature;
  float lux_value;
  double temperature_outside;
  float rain_coefficient;
  float uv_index;
  float pressure;
  uint32_t vibration_count;
} outside_station;

typedef enum {
  STATION_OUTSIDE = 1,
  STATION_KITCHEN = 2,
  STATION_ROOM = 3
} station_number;

void data_transmission_initialization(void);
bool data_transmission(outside_station *data);

#endif