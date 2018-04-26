#include <atmel_start.h>
#include <stdarg.h>
#include <stdio.h>

#define USB_PRINT()

/**
 * \brief Print formatted text over USB.
 */
static int usb_printf(const char *fmt, ...)
{
    char text[255];

    va_list args;
    va_start(args, fmt);
    vsprintf(text, fmt, args);
    va_end(args);

    return cdcdf_acm_write(text, strlen(text));
}

/**
 * \brief Reset the device.
 */
static void reset()
{
    NVIC_SystemReset();
    while (true);
}

#define FLASH_APP_START (0x00002000)

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

static uint8_t usb_buffer[CONF_USB_CDCD_ACM_DATA_BULKIN_MAXPKSZ];

/**
 * \brief Callback invoked when host request bulk read.
 */
static bool usb_cb_read(const uint8_t ep, const enum usb_xfer_code rc, const uint32_t count)
{
    cdcdf_acm_write((uint8_t *)usb_buffer, count);

    /* No error. */
    return false;
}

/**
 * \brief Callback invoked when host request bulk write.
 */
static bool usb_cb_write(const uint8_t ep, const enum usb_xfer_code rc, const uint32_t count)
{
    cdcdf_acm_read((uint8_t *)usb_buffer, sizeof(usb_buffer));

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
        gpio_set_pin_level(LED_BUILTIN, 1);
        /* Data Terminal Ready */
        cdcdf_acm_register_callback(CDCDF_ACM_CB_READ, (FUNC_PTR)usb_cb_read);
        cdcdf_acm_register_callback(CDCDF_ACM_CB_WRITE, (FUNC_PTR)usb_cb_write);
    } else {
        gpio_set_pin_level(LED_BUILTIN, 0);
        /* Erase and reset when the port is opened then closed at 1200 bps */
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
        /* Wait cdc acm to be enabled */
    };

    /* Register callback on USB CDC state change */
    cdcdf_acm_register_callback(CDCDF_ACM_CB_STATE_C, (FUNC_PTR)usb_cb_state_c);

    uint8_t count = 0;

    while (1) {
        delay_ms(500);

        /* Print over USB serial */
        usb_printf("blink: %d\n", count++);
    }
}