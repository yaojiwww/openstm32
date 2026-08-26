#include "board_port.h"
#include "fault.h"
#include "storage.h"

static FaultManager faults;
static uint32_t next_sd_retry_ms;

void lab9_init(void) { fault_init(&faults); }

void lab9_on_sd_error(void) {
    fault_raise(&faults, FAULT_SD_MISSING);
    next_sd_retry_ms = board_millis() + 1000u;
}

void lab9_loop(void) {
    if (fault_is_active(&faults, FAULT_SD_MISSING) &&
        (int32_t)(board_millis() - next_sd_retry_ms) >= 0) {
        next_sd_retry_ms = board_millis() + 1000u;
        if (storage_mount()) {
            fault_clear(&faults, FAULT_SD_MISSING);
            board_log("SD recovered\r\n");
        } else {
            board_log("retry SD\r\n");
        }
    }
    board_watchdog_refresh();
}
