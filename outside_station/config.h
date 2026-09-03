#ifndef OUTSIDE_STATION_CONFIG_H
#define OUTSIDE_STATION_CONFIG_H

#define PASSWORD "12345678"
#define WIFI_NAME "PROJECT"
#define IP_MASTER "192.168.4.1"
#define MASTER_PORT 4444


//analog pins

#define fc_37_pin 28 //rain sensor
#define guva12sd_pin 26 // UV sensor


//digital pins
#define ds18b20_pin 22
#define bh1750_sda0_pin 4
#define bh1750_scl0_pin 5
#define aht20_bmp280_sda1_pin 14
#define aht20_bmp280_scl1_pin 15
#define sw420_pin 16 //vibration sensor


#define MEASUREMENT_INTERVAL 60000  // 60 seconds

typedef enum wind_state{
    SMALL_WIND = 5,
    MEDIUM_WIND = 30,
    STRONG_WIND = 100,
    EXTREME_WIND = 250
}wind_state;

typedef enum rain{
    DRY = 10,
    DRIZZLE = 35,
    MODERATE = 70
}rain_amount;


#endif