#include "board_port.h"
#include "workstation.h"

#include <stdio.h>

static Workstation station;

void lab6_init(const AppConfig *config) { workstation_init(&station, config); }

void lab6_event(AppEvent event) {
    WorkMode before = station.mode;
    workstation_dispatch(&station, event);
    if (before != station.mode) {
        char line[64];
        (void)snprintf(line, sizeof(line), "STATE %s -> %s\r\n",
                       workstation_mode_name(before),
                       workstation_mode_name(station.mode));
        board_log(line);
    }
}

void lab6_loop(void) {
    if (station.entry_pending) {
        station.entry_pending = false;
        if (station.mode == MODE_STANDBY) board_enter_stop_mode();
    }
    /* Each mode's real work calls the already completed lab2-lab5 modules. */
    board_watchdog_refresh();
}
