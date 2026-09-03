#include <stdio.h>
#include <pico/stdlib.h>
#include <hardware/adc.h>
#include "config.h"
#include "mq6.h"
#include "mq7.h"
#include "mq135.h"
#include "max4466.h"
#include "rgb.h"
#include "sharp_gp2y.h"
#include "second_rgb.h"
#include "data_transmission2.h"

int main() {
    stdio_init_all();
    sleep_ms(2000);
    rgb_initialize();
    second_rgb_initialize();
    max4466_initialize(max4466_pin);
    sharp_gp2y_initialize();
    mq6_initialize(mq6_pin);
    mq7_initialize(mq7_pin);
    mq135_initialize(mq135_pin);

    data_transmission2_initialization();

    while (true) {
        float decibel_amount = max4466_decibel_level(max4466_pin);
        float dust_density = dust_density_level();
        bool is_gas_detected = is_mq6_alarm(mq6_pin);
        bool is_carbon_monoxide_detected = is_mq7_alarm(mq7_pin);
        float carbon_dioxide_level = mq135_get_ppm(mq135_pin);

        if(carbon_dioxide_level < 1000.0f){
            rgb_set_color(0, 1, 0); //green
        } else if (carbon_dioxide_level <= 1500.0f) {
            rgb_set_color(1, 1, 0); //yellow
        } else {
            rgb_set_color(1, 0, 0); //red
        }

        if (dust_density < 35.0f) {
            second_rgb_set_color(0, 1, 0); //green
        } else if (dust_density < 75.0f) {
            second_rgb_set_color(1, 1, 0); //yellow
        } else {
            second_rgb_set_color(1, 0, 0); //red
        }

        if (is_carbon_monoxide_detected) {
            rgb_set_color(1, 0, 0);
            second_rgb_set_color(1, 0, 0);
            sleep_ms(250);
            rgb_set_color(0, 0, 0);
            second_rgb_set_color(0, 0, 0);
            sleep_ms(250);
        } else if (is_gas_detected) {
            rgb_set_color(0, 0, 1);
            second_rgb_set_color(0, 0, 1);
            sleep_ms(250);
            rgb_set_color(0, 0, 0);
            second_rgb_set_color(0, 0, 0);
            sleep_ms(250);
        }
        /*
        printf("Decibel level: %.1f dB\n", decibel_amount);
        printf("Dust level: %.1f ug/m3\n", dust_density);
        printf("CO2 level: CO2: %.0f PPM\n", carbon_dioxide_level);
        if(!is_carbon_monoxide_detected){
            printf("CO not detected; CO flag: \n", is_carbon_monoxide_detected);
        }
        else{
            printf("WARNING! CO DETECTED! CO FLAG: %d\n", is_carbon_monoxide_detected);
        }
        if(!is_gas_detected){
            printf("Gas leak not detected, Gas flag: %d\n", is_gas_detected );
        }
        else{
            printf("WARNING! GAS LEAK DETECTED! GAS FLAG: %d\n", is_gas_detected);
        }
        fflush(stdout);
        */
        kitchen_data k_data = {
            .station_id           = KITCHEN_STATION,
            .decibel_level        = decibel_amount,
            .carbon_dioxide_level = carbon_dioxide_level,
            .dust_density         = dust_density,
            .is_carbon_monoxide   = is_carbon_monoxide_detected,
            .is_gas_leak          = is_gas_detected,
        };
        data_transmission2(&k_data);
        sleep_ms(1000);
    }
    return 0;
}


