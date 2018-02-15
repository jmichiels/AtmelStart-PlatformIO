#include <atmel_start.h>

int main(void) {
    /* Initializes MCU, drivers and middleware */
    atmel_start_init();

    /* USB 'Echo' example */
    cdcd_acm_example();
}
