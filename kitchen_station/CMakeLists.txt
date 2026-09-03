add_executable(kitchen_station
    main.c
    max4466.c
    mq6.c
    mq7.c
    mq135.c
    rgb.c
    sharp_gp2y.c
    data_transmission2.c
    second_rgb.c
)

target_include_directories(kitchen_station PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}
)


target_link_libraries(kitchen_station
    pico_stdlib
    hardware_adc
    hardware_i2c
    hardware_gpio
    hardware_pwm
    pico_cyw43_arch_lwip_threadsafe_background

)

pico_enable_stdio_usb(kitchen_station 1)
pico_enable_stdio_uart(kitchen_station 0)
pico_add_extra_outputs(kitchen_station)