#include "workstation.h"

void workstation_init(Workstation *station, const AppConfig *config) {
    station->mode = MODE_AUTO_SAMPLE;
    station->config = *config;
    station->running = false;
    station->sample_requested = false;
    station->entry_pending = true;
    station->transition_count = 0;
}

static void enter_mode(Workstation *station, WorkMode next) {
    station->mode = next;
    station->sample_requested = false;
    station->entry_pending = true;
    ++station->transition_count;
}

void workstation_dispatch(Workstation *station, AppEvent event) {
    if (event == EVENT_FAULT || event == EVENT_STOP) {
        station->running = false;
        station->sample_requested = false;
        return;
    }
    if (event == EVENT_MODE_BUTTON && station->mode != MODE_STANDBY) {
        enter_mode(station, (WorkMode)((station->mode + 1) % MODE_COUNT));
        return;
    }

    switch (station->mode) {
    case MODE_AUTO_SAMPLE:
        if (event == EVENT_TIMER_TICK && station->running)
            station->sample_requested = true;
        break;
    case MODE_MANUAL_SAMPLE:
        if (event == EVENT_SAMPLE_BUTTON && station->running &&
            !station->sample_requested)
            station->sample_requested = true;
        break;
    case MODE_REPLAY:
        if (event == EVENT_REPLAY_DONE) station->running = false;
        break;
    case MODE_STANDBY:
        if (event == EVENT_WAKEUP) {
            station->running = true;
            enter_mode(station, MODE_AUTO_SAMPLE);
        }
        break;
    default:
        enter_mode(station, MODE_AUTO_SAMPLE);
        break;
    }
}

const char *workstation_mode_name(WorkMode mode) {
    static const char *names[MODE_COUNT] = {"AUTO", "MANUAL", "REPLAY", "STANDBY"};
    return (mode < MODE_COUNT) ? names[mode] : "UNKNOWN";
}
