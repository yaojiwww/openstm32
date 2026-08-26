#include "board_port.h"
#include "sampler.h"
#include "storage.h"

static Sampler sampler;
static volatile bool sample_due;

void lab2_init(const AppConfig *config) {
    sampler_init(&sampler, config->filter_window, 3300, 4095);
    if (!storage_mount()) board_log("ERR: SD mount\r\n");
    board_sample_timer_start(config->sample_rate_hz);
}

/* Call from HAL_TIM_PeriodElapsedCallback when htim->Instance == TIM2. */
void lab2_on_timer(void) { sample_due = true; }

void lab2_loop(void) {
    if (!sample_due) return;
    sample_due = false;
    uint16_t raw;
    if (!board_adc_read(&raw)) {
        board_log("ERR: ADC timeout\r\n");
        return;
    }
    SampleRecord record = sampler_process(&sampler, raw, board_millis());
    if (!storage_append(&record)) board_log("ERR: SD write\r\n");
}
