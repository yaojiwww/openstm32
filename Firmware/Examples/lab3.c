#include "board_port.h"
#include "sampler.h"

#include <stdio.h>

static Sampler sampler;
static volatile bool trigger_pending;
static bool sampling;

void lab3_init(const AppConfig *config) {
    sampler_init(&sampler, config->filter_window, 3300, 4095);
}

void lab3_on_key_interrupt(void) {
    if (!sampling) trigger_pending = true;
}

void lab3_loop(void) {
    if (!trigger_pending || sampling) return;
    trigger_pending = false;
    sampling = true;
    uint16_t raw;
    char message[64];
    if (board_adc_read(&raw)) {
        SampleRecord record = sampler_process(&sampler, raw, board_millis());
        (void)snprintf(message, sizeof(message), "sample=%u, %u mV\r\n",
                       record.filtered, record.millivolts);
        board_log(message);
        board_buzzer_beep(20);
    } else {
        board_log("ERR: ADC timeout\r\n");
    }
    sampling = false;
}
