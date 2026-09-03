#ifndef DATATRANSMISSION2_H
#define DATATRANSMISSION2_H

#include "config.h"
#include <stdbool.h>


typedef struct __attribute__((packed)){
    uint8_t station_id;
    float decibel_level;
    float carbon_dioxide_level;
    float dust_density;
    bool is_carbon_monoxide;
    bool is_gas_leak;
}kitchen_data;

typedef enum {
    STATION_OUTSIDE = 1,
    STATION_KITCHEN = 2,
    STATION_ROOM    = 3
} station_number;

void data_transmission2_initialization(void);
bool data_transmission2(kitchen_data *data);


#endif