add_executable(outside_station
    main.c
    aht20.c
    bmp280.c
    data_transmission.c
    fc37.c
    guva_s12sd.c
    sw420.c
    ds18b20.c
    bh1750.c
)

target_include_directories(outside_station PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}
)

target_link_libraries(outside_station
    pico_stdlib
    hardware_adc
    hardware_gpio
    hardware_i2c
    pico_cyw43_arch_lwip_threadsafe_background
)

pico_enable_stdio_usb(outside_station 1)
pico_add_extra_outputs(outside_station)