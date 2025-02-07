#ifndef SSD1306_H
#define SSD1306_H

#include "pico/stdlib.h"
#include "hardware/i2c.h"

void ssd1306_init();
void ssd1306_clear();
void ssd1306_display();
void ssd1306_draw_char(char c);
void ssd1306_set_cursor(uint8_t x, uint8_t y);
void ssd1306_write_string(const char *str);

#endif // SSD1306_H
