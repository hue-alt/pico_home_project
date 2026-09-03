#ifndef MY_ROOM_STATION_CONFIG_H
#define MY_ROOM_STATION_CONFIG_H

#define PASSWORD "12345678"
#define WIFI_NAME "PROJECT"
#define IP_MASTER "192.168.4.1"
#define MASTER_PORT 4444


//analog pins
#define max4466_pin 27
#define temt6000_pin 26

//digital pins
#define button_first_screen_pin 0
#define button_second_screen_pin 13
#define kitchen_screen_and_ds3231_sda0_pin 4
#define kitchen_screen_and_ds3231_scl0_pin 5
#define aht20_bmp280_sda1_pin 2
#define aht20_bmp280_scl1_pin 3
#define outside_and_room_screen_sda_pin 2
#define outside_and_room_screen_scl_pin 3
#define ds3231_sda_pin 4
#define ds3231_scl_pin 5



#define MEASUREMENT_INTERVAL 1000 //1 second

#endif