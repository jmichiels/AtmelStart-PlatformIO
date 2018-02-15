#include <atmel_start.h>

int main(void) {
    /* Initializes MCU, drivers and middleware */
    atmel_start_init();

    while (1) {
        delay_ms(1000);

        // Just blink the built-in led.
        gpio_toggle_pin_level(LED_BUILTIN);
    }
}
