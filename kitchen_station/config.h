#ifndef KITCHEN_STATION_CONFIG_H
#define KITCHEN_STATION_CONFIG_H

#define PASSWORD "12345678"
#define WIFI_NAME "PROJECT"
#define IP_MASTER "192.168.4.1"
#define MASTER_PORT 4444


#define OUTSIDE_STATION 1
#define KITCHEN_STATION 2
#define ROOM_STATION 3


//analog pins
#define sharp_pin 28
#define max4466_pin 27
#define mq135_pin 26

//digital pins
#define sharp_led_pin 20
#define mq7_pin 18
#define mq6_pin 16


//diode pins
#define diode_red_pin 13
#define diode_green_pin 14
#define diode_blue_pin 15
#define second_diode_red_pin 21
#define second_diode_green_pin 22
#define second_diode_blue_pin 17



#define measurement_interval 5000 // 5seconds
#endif 