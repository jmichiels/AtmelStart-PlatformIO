#include <atmel_start.h>

int main(void) {
    /* Initializes MCU, drivers and middleware */
    atmel_start_init();

    /* USB 'Echo' example */
    // cdcd_acm_example();

    while (1) {
        delay_ms(500);

        // Just blink the built-in led.
        gpio_toggle_pin_level(LED_BUILTIN);
    }
}
