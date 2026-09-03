#include <pico/stdlib.h>
#include <hardware/adc.h>
#include <hardware/i2c.h>
#include <stdio.h>
#include "config.h"
#include "aht20.h"
#include "bh1750.h"
#include "bmp280.h"
#include "ds18b20.h"
#include "fc37.h"
#include "guva_s12sd.h"
#include "sw420.h"
#include "data_transmission.h"


int main(){
    stdio_init_all();
    sleep_ms(2000);
    bh1750_initialization(i2c0);
    aht20_initialization(i2c1);
    bmp280_initalization(i2c1);
    DS18B20_intialization(ds18b20_pin);
    fc37_initialize(fc_37_pin);
    guva12sd_initialize(guva12sd_pin);
    sw420_initialize();

    data_transmission_initialization();

    while(true){
        float aht_temperature = 0.0f;
        float humidity = 0.0f;
        get_temp_and_humidity(i2c1, &aht_temperature, &humidity);
        float pressure = 0.00f;
        float bmp_280_temperature = 0.0f;
        bmp280_get_temp_and_pressure(i2c1, &bmp_280_temperature, &pressure);
        float temperature_inside_box = ((aht_temperature) + (bmp_280_temperature))/2.0f; //temperature inside box with pico
        float temperature_outside = DS18B20_get_temperature(ds18b20_pin);
        float uv_value = get_uv_value(guva12sd_pin);
        float get_lux = bh1750_get_lux(i2c0);
        uint32_t vibrations = sw420_get_result();
        int rain_coefficient = get_rain_coefficient(fc_37_pin);

        /*
        printf("Temperature outside: %.1f C\n", temperature_outside);
        printf("Temperature inside the box: %1.f C\n", temperature_inside_box);
        printf("Pressure: %2.f Hpa\n", pressure);
        printf("Uv value: %2.f UV\n", uv_value);
        printf("Lux value: %2.f Lux\n", get_lux);


        if(vibrations <= SMALL_WIND){
            printf("No wind detected.\n");
        }else if(vibrations <= SMALL_WIND && vibrations < MEDIUM_WIND){
            printf("Small wind detected.\n");
        }else if( vibrations <= STRONG_WIND){
            printf("Medium wind detected.\n");
        }else if(vibrations <= EXTREME_WIND){
            printf("STRONG WIND DETECTED!\n");
        }else{
            printf("EXTREME WIND DETECTED!\n");
        }
        
        if(rain_coefficient <= DRY){
            printf("No rain detected.\n");
        }else if(rain_coefficient <= DRIZZLE){
            printf("Drizzle outside.\n");
        }else if(rain_coefficient <= MODERATE){
            printf("Moderate rain outside.\n");
        }else{
            printf("RAINFALL!\n");
        }
        
        */
        outside_station o_station = {
            .station_id = STATION_OUTSIDE,
            .humidity = humidity,
            .lux_value = get_lux,
            .temperature = temperature_inside_box,
            .temperature_outside = temperature_outside,
            .uv_index = uv_value,
            .vibration_count = vibrations,
            .rain_coefficient = rain_coefficient,
            .pressure = pressure
        };

        data_transmission(&o_station);
        sleep_ms(MEASUREMENT_INTERVAL);
    }
    return 0;
}
