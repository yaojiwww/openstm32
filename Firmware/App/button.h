#ifndef BUTTON_H
#define BUTTON_H

#include "app_types.h"

typedef enum {
    BUTTON_EVENT_NONE = 0,
    BUTTON_EVENT_CLICK,
    BUTTON_EVENT_LONG_PRESS
} ButtonEvent;

typedef struct {
    bool stable_level;
    bool candidate_level;
    uint32_t candidate_since_ms;
    uint32_t pressed_since_ms;
    uint16_t debounce_ms;
    uint16_t long_press_ms;
    bool long_reported;
} Button;

void button_init(Button *button, bool released_level, uint16_t debounce_ms,
                 uint16_t long_press_ms);
ButtonEvent button_update(Button *button, bool raw_level, uint32_t now_ms);

#endif
