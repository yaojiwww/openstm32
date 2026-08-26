#include "board_port.h"
#include "button.h"

static Button key;
static AppConfig config;

void lab1_init(void) {
    StoredConfig stored;
    button_init(&key, true, 20, 800);
    if (!board_config_load(&stored) || !config_unpack(&stored, &config)) {
        config = config_defaults();
        stored = config_pack(&config);
        (void)board_config_save(&stored);
    }
}

void lab1_loop(void) {
    ButtonEvent event = button_update(&key, !board_key_pressed(), board_millis());
    if (event == BUTTON_EVENT_CLICK) {
        config.sample_rate_hz = config.sample_rate_hz >= 1000
                                    ? 10 : config.sample_rate_hz * 10;
        StoredConfig stored = config_pack(&config);
        (void)board_config_save(&stored);
        board_buzzer_beep(30);
    } else if (event == BUTTON_EVENT_LONG_PRESS) {
        config = config_defaults();
        StoredConfig stored = config_pack(&config);
        (void)board_config_save(&stored);
        board_buzzer_beep(200);
    }
}
