#include "button.h"

void button_init(Button *button, bool released_level, uint16_t debounce_ms,
                 uint16_t long_press_ms) {
    button->stable_level = released_level;
    button->candidate_level = released_level;
    button->candidate_since_ms = 0;
    button->pressed_since_ms = 0;
    button->debounce_ms = debounce_ms;
    button->long_press_ms = long_press_ms;
    button->long_reported = false;
}

ButtonEvent button_update(Button *button, bool raw_level, uint32_t now_ms) {
    if (raw_level != button->candidate_level) {
        button->candidate_level = raw_level;
        button->candidate_since_ms = now_ms;
    }

    if ((raw_level != button->stable_level) &&
        ((uint32_t)(now_ms - button->candidate_since_ms) >= button->debounce_ms)) {
        button->stable_level = raw_level;
        if (!raw_level) {
            button->pressed_since_ms = now_ms;
            button->long_reported = false;
        } else if (!button->long_reported) {
            return BUTTON_EVENT_CLICK;
        }
    }

    if (!button->stable_level && !button->long_reported &&
        ((uint32_t)(now_ms - button->pressed_since_ms) >= button->long_press_ms)) {
        button->long_reported = true;
        return BUTTON_EVENT_LONG_PRESS;
    }
    return BUTTON_EVENT_NONE;
}
