#include <atmel_start.h>
#include <usb/class/cdc/usb_protocol_cdc.h>
#include <samd21a/include/component/nvmctrl.h>


#define FLASH_APP_START (0x00002000)

/**
 * \brief Reset the device.
 */
static void reset()
{
    NVIC_SystemReset();
    while (true);
}

/**
 * \brief Erase the application then reset the device.
 */
__attribute__ ((long_call, section (".ramfunc")))
static void erase()
{
    if (flash_erase(&FLASH_0, FLASH_APP_START, 1) == ERR_NONE) {
        reset();
    }
}

/**
 * \brief Callback invoked when host read.
 */
static bool usb_cb_read(const uint8_t ep, const enum usb_xfer_code rc, const uint32_t count)
{
    gpio_set_pin_level(LED_BUILTIN, 1);
    uint8_t buffer[] = "hello\n";
    cdcdf_acm_write(buffer, strlen(buffer));

    /* No error. */
    return false;
}

#define USB_RESET_DATARATE (1200)

/**
 * \brief Callback invoked when line state changes
 */
static bool usb_cb_state_c(usb_cdc_control_signal_t state)
{
    if (state.rs232.DTR) {
        /* Data Terminal Ready */
        // cdcdf_acm_register_callback(CDCDF_ACM_CB_WRITE, (FUNC_PTR)usb_cb_read);
    } else {
        /* Auto-reset into the bootloader is triggered when the port, already open at 1200 bps, is closed */
        if (cdcdf_acm_get_line_coding()->dwDTERate == USB_RESET_DATARATE) {
            erase();
        }
    }
    /* No error. */
    return false;
}

int main(void)
{
    /* Initializes MCU, drivers and middleware */
    atmel_start_init();

    while (!cdcdf_acm_is_enabled()) {
        // wait cdc acm to be installed
    };

    cdcdf_acm_register_callback(CDCDF_ACM_CB_STATE_C, (FUNC_PTR)usb_cb_state_c);

    while (1) {
        delay_ms(500);

        // Just blink the built-in led.
       gpio_toggle_pin_level(LED_BUILTIN);
    }
}
