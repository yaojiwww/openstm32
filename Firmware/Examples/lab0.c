#include "board_port.h"

/* Call once after CubeMX MX_*_Init functions. */
void lab0_init(void) {
    board_led_set(true);
    board_log("lab0 ready\r\n");
}

/* Call from HAL_GPIO_EXTI_Callback when GPIO_Pin == KEY_Pin. */
void lab0_on_key_interrupt(void) {
    static uint32_t last_ms;
    uint32_t now = board_millis();
    if ((uint32_t)(now - last_ms) >= 20u) {
        board_led_toggle();
        last_ms = now;
    }
}
