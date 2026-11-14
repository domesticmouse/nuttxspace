#include <stdio.h>
#include "pico/stdlib.h"

int main(void) {
    stdio_init_all();

#ifdef PICO_DEFAULT_LED_PIN
    gpio_init(PICO_DEFAULT_LED_PIN);
    gpio_set_dir(PICO_DEFAULT_LED_PIN, GPIO_OUT);
#endif

    uint32_t tick = 0;
    while (true) {
#ifdef PICO_DEFAULT_LED_PIN
        gpio_put(PICO_DEFAULT_LED_PIN, tick & 1u);
#endif
        printf("Hello from raw Pico SDK! (tick=%lu)\r\n", (unsigned long)tick++);
        sleep_ms(1000);
    }
}
