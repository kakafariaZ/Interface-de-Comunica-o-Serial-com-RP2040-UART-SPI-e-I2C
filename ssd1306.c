#include "ssd1306.h"
#include <string.h>

// Defina os comandos do SSD1306
#define SSD1306_ADDRESS 0x3C
#define SSD1306_WIDTH 128
#define SSD1306_HEIGHT 64

static uint8_t buffer[SSD1306_WIDTH * SSD1306_HEIGHT / 8];
static uint8_t cursor_x = 0;
static uint8_t cursor_y = 0;

void ssd1306_write_command(uint8_t cmd) {
    uint8_t data[2] = {0x00, cmd};
    i2c_write_blocking(i2c_default, SSD1306_ADDRESS, data, 2, false);
}

void ssd1306_init() {
    // Inicialização do display
    ssd1306_write_command(0xAE); // Display OFF
    ssd1306_write_command(0x20); // Set Memory Addressing Mode
    ssd1306_write_command(0x00); // Horizontal Addressing Mode
    ssd1306_write_command(0xB0); // Set Page Start Address for Page Addressing Mode
    ssd1306_write_command(0xC8); // COM Output Scan Direction
    ssd1306_write_command(0x00); // Set Low Column Address
    ssd1306_write_command(0x10); // Set High Column Address
    ssd1306_write_command(0x40); // Set Start Line Address
    ssd1306_write_command(0x81); // Set Contrast Control
    ssd1306_write_command(0xFF); // Maximum contrast
    ssd1306_write_command(0xA1); // Set Segment Re-map
    ssd1306_write_command(0xA6); // Set Normal Display
    ssd1306_write_command(0xA8); // Set Multiplex Ratio
    ssd1306_write_command(0x3F);
    ssd1306_write_command(0xA4); // Entire Display ON
    ssd1306_write_command(0xD3); // Set Display Offset
    ssd1306_write_command(0x00);
    ssd1306_write_command(0xD5); // Set Display Clock Divide Ratio/Oscillator Frequency
    ssd1306_write_command(0xF0); // Increase speed
    ssd1306_write_command(0xD9); // Set Pre-charge Period
    ssd1306_write_command(0x22);
    ssd1306_write_command(0xDA); // Set COM Pins Hardware Configuration
    ssd1306_write_command(0x12);
    ssd1306_write_command(0xDB); // Set VCOMH Deselect Level
    ssd1306_write_command(0x20);
    ssd1306_write_command(0x8D); // Charge Pump Setting
    ssd1306_write_command(0x14);
    ssd1306_write_command(0xAF); // Display ON

    ssd1306_clear();
    ssd1306_display();
}

void ssd1306_clear() {
    memset(buffer, 0, sizeof(buffer));
    cursor_x = 0;
    cursor_y = 0;
}

void ssd1306_display() {
    for (uint8_t i = 0; i < 8; i++) {
        ssd1306_write_command(0xB0 + i);
        ssd1306_write_command(0x00);
        ssd1306_write_command(0x10);
        i2c_write_blocking(i2c_default, SSD1306_ADDRESS, buffer + SSD1306_WIDTH * i, SSD1306_WIDTH, false);
    }
}

void ssd1306_draw_char(char c) {
    // Implemente a função para desenhar um caractere específico
    // Usando a biblioteca de fontes definida anteriormente (font.h)
}

void ssd1306_set_cursor(uint8_t x, uint8_t y) {
    cursor_x = x;
    cursor_y = y;
}

void ssd1306_write_string(const char *str) {
    while (*str) {
        ssd1306_draw_char(*str++);
    }
}
