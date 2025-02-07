#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/uart.h"
#include "hardware/i2c.h"
#include "hardware/gpio.h"
#include "hardware/irq.h"
#include "font.h"
#include "ws2812.h"
#include "ssd1306.h"

// Definições das GPIOs
#define BUTTON_A 5
#define BUTTON_B 6
#define LED_RGB_R 11
#define LED_RGB_G 12
#define LED_RGB_B 13
#define WS2812_PIN 7
#define I2C_SDA 14
#define I2C_SCL 15

// Variáveis globais de estado dos botões
volatile bool button_a_pressed = false;
volatile bool button_b_pressed = false;

// Configurações de debouncing
absolute_time_t last_interrupt_time_a = {0};
absolute_time_t last_interrupt_time_b = {0};
const uint32_t debounce_us = 200000; // 200 ms

// Funções de callback de interrupção
void gpio_callback_a(uint gpio, uint32_t events) {
    if (absolute_time_diff_us(last_interrupt_time_a, get_absolute_time()) < debounce_us) return;
    last_interrupt_time_a = get_absolute_time();
    button_a_pressed = !button_a_pressed;
    gpio_put(LED_RGB_G, button_a_pressed);
}

void gpio_callback_b(uint gpio, uint32_t events) {
    if (absolute_time_diff_us(last_interrupt_time_b, get_absolute_time()) < debounce_us) return;
    last_interrupt_time_b = get_absolute_time();
    button_b_pressed = !button_b_pressed;
    gpio_put(LED_RGB_B, button_b_pressed);
}

void setup() {
    // Configuração das GPIOs dos LEDs
    gpio_init(LED_RGB_R);
    gpio_set_dir(LED_RGB_R, GPIO_OUT);
    gpio_init(LED_RGB_G);
    gpio_set_dir(LED_RGB_G, GPIO_OUT);
    gpio_init(LED_RGB_B);
    gpio_set_dir(LED_RGB_B, GPIO_OUT);

    // Configuração dos botões com interrupções
    gpio_set_irq_enabled_with_callback(BUTTON_A, GPIO_IRQ_EDGE_FALL, true, &gpio_callback_a);
    gpio_set_irq_enabled_with_callback(BUTTON_B, GPIO_IRQ_EDGE_FALL, true, &gpio_callback_b);

    // Configuração da comunicação serial
    stdio_init_all();
    uart_init(uart0, 115200);
    gpio_set_function(PICO_DEFAULT_UART_TX_PIN, GPIO_FUNC_UART);
    gpio_set_function(PICO_DEFAULT_UART_RX_PIN, GPIO_FUNC_UART);

    // Configuração do display SSD1306
    i2c_init(i2c_default, 400 * 1000);
    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SDA);
    gpio_pull_up(I2C_SCL);
    ssd1306_init();
    ssd1306_clear();

    // Configuração dos LEDs WS2812
    ws2812_init(WS2812_PIN);
}

void loop() {
    char c;
    while (true) {
        // Leia o caractere do monitor serial
        if (uart_is_readable(uart0)) {
            c = uart_getc(uart0);
            ssd1306_draw_char(c);
            ssd1306_display();

            // Exiba o símbolo correspondente na matriz 5x5 de LEDs WS2812 se for um número
            if (c >= '0' && c <= '9') {
                ws2812_show_number(c - '0');
            }
        }

        // Verifique os botões e atualize o display
        if (button_a_pressed) {
            ssd1306_set_cursor(0, 0);
            ssd1306_write_string("LED Verde: Ligado");
            uart_puts(uart0, "LED Verde: Ligado\n");
        } else {
            ssd1306_set_cursor(0, 0);
            ssd1306_write_string("LED Verde: Desligado");
            uart_puts(uart0, "LED Verde: Desligado\n");
        }

        if (button_b_pressed) {
            ssd1306_set_cursor(0, 1);
            ssd1306_write_string("LED Azul: Ligado");
            uart_puts(uart0, "LED Azul: Ligado\n");
        } else {
            ssd1306_set_cursor(0, 1);
            ssd1306_write_string("LED Azul: Desligado");
            uart_puts(uart0, "LED Azul: Desligado\n");
        }

        sleep_ms(100);
    }
}

int main() {
    setup();
    loop();
    return 0;
}
