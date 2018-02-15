#include <atmel_start.h>

int main(void) {
    /* Initializes MCU, drivers and middleware */
    atmel_start_init();

    while (1) {
        delay_ms(1000);
        // Just blink to make sure it works.
        gpio_toggle_pin_level(LED_BUILTIN);
    }

    /* USB 'Echo' example */
    // cdcd_acm_example();
}
