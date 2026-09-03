add_executable(my_room_station
    buttons.c
    aht20.c
    main.c
    datareception.c
    ds3231.c
    max4466.c
    screens.c
    temt6000.c
    ssd1306.c
)

target_include_directories(my_room_station PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}
)

target_link_libraries(my_room_station
    pico_stdlib
    hardware_adc
    hardware_gpio
    hardware_i2c
    pico_cyw43_arch_lwip_threadsafe_background
)

pico_enable_stdio_usb(my_room_station 1)
pico_add_extra_outputs(my_room_station)