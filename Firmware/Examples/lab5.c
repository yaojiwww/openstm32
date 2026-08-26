#include "board_port.h"

void lab5_loop(bool standby_requested) {
    if (standby_requested) {
        board_log("enter STOP mode\r\n");
        board_sample_timer_stop();
        board_led_set(false);
        board_enter_stop_mode();
        board_log("wakeup\r\n");
    }
    board_watchdog_refresh();
}
