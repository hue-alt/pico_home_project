#ifndef SW420_H
#define SW420_H

void sw420_initialize(void);
uint32_t sw420_get_result(void);
static void pin_checker(uint pin, uint32_t bitmask);
#endif