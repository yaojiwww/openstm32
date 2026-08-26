#ifndef WORKSTATION_H
#define WORKSTATION_H

#include "app_types.h"

typedef enum {
    EVENT_NONE = 0,
    EVENT_MODE_BUTTON,
    EVENT_SAMPLE_BUTTON,
    EVENT_TIMER_TICK,
    EVENT_REPLAY_DONE,
    EVENT_WAKEUP,
    EVENT_STOP,
    EVENT_FAULT
} AppEvent;

typedef struct {
    WorkMode mode;
    AppConfig config;
    bool running;
    bool sample_requested;
    bool entry_pending;
    uint32_t transition_count;
} Workstation;

void workstation_init(Workstation *station, const AppConfig *config);
void workstation_dispatch(Workstation *station, AppEvent event);
const char *workstation_mode_name(WorkMode mode);

#endif
