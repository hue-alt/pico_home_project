#include <pico/stdlib.h>
#include <hardware/sync.h>
#include <stdbool.h>
#include "config.h"
#include "sw420.h"

static volatile uint32_t cnt = 0; //counting flag visible for this file

static void pin_checker(uint pin, uint32_t bitmask){ 
    (void)bitmask;
    if(pin == sw420_pin){
        cnt++;
    }
}

void sw420_initialize(void){
    gpio_init(sw420_pin);
    gpio_set_dir(sw420_pin, GPIO_IN);
    gpio_pull_down(sw420_pin);
    gpio_set_irq_enabled_with_callback(sw420_pin, GPIO_IRQ_EDGE_RISE, true, &pin_checker); //state checker
}

uint32_t sw420_get_result(void){
    uint32_t ints = save_and_disable_interrupts();
    uint32_t vibration_count = cnt;
    cnt = 0;
    restore_interrupts(ints);
    return vibration_count;
}