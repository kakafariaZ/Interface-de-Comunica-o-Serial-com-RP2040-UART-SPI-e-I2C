#include "ws2812.h"
#include "hardware/pio.h"
#include "hardware/clocks.h"
#include "ws2812.pio.h"

void ws2812_program_init(PIO pio, uint sm, uint offset, uint pin, uint freq, bool sideset) {
    pio_sm_config c = ws2812_program_get_default_config(offset);
    sm_config_set_wrap(&c, offset + ws2812_wrap_target, offset + ws2812_wrap);
    sm_config_set_sideset(&c, 1, false, false);
    sm_config_set_out_shift(&c, false, true, 24);
    sm_config_set_fifo_join(&c, PIO_FIFO_JOIN_TX);
    sm_config_set_clkdiv(&c, (float)clock_get_hz(clk_sys) / (freq * 3));
    pio_sm_init(pio, sm, offset, &c);
    pio_sm_set_enabled(pio, sm, true);
}

void ws2812_init(uint pin) {
    PIO pio = pio0;
    int sm = 0;
    uint offset = pio_add_program(pio, &ws2812_program);
    ws2812_program_init(pio, sm, offset, pin, 800000, false);
}

void ws2812_show_number(uint8_t number) {
    uint32_t colors[10] = {
        0x0000FF, // 0 - Azul
        0x00FF00, // 1 - Verde
        0xFF0000, // 2 - Vermelho
        0xFFFF00, // 3 - Amarelo
        0x00FFFF, // 4 - Ciano
        0xFF00FF, // 5 - Magenta
        0xFFFFFF, // 6 - Branco
        0x888888, // 7 - Cinza
        0x444444, // 8 - Cinza escuro
        0x000000  // 9 - Apagado
    };

    PIO pio = pio0;
    int sm = 0;
    uint32_t color = colors[number % 10];
    pio_sm_put_blocking(pio, sm, color << 8u);
}
