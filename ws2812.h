#ifndef WS2812_H
#define WS2812_H

#include "pico/stdlib.h"

void ws2812_init(uint pin);
void ws2812_show_number(uint8_t number);

#endif // WS2812_H
