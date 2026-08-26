#ifndef BOARD_PORT_H
#define BOARD_PORT_H

#include "app_types.h"
#include "config.h"

/* All functions use CubeMX user labels, not hard-coded board pin numbers. */
void board_led_set(bool on);
void board_led_toggle(void);
void board_buzzer_beep(uint16_t duration_ms);
bool board_key_pressed(void);

uint32_t board_millis(void);
void board_delay_ms(uint32_t duration_ms);
void board_log(const char *text);
bool board_uart_send(const uint8_t *data, size_t length);

bool board_adc_read(uint16_t *raw);
bool board_config_load(StoredConfig *config);
bool board_config_save(const StoredConfig *config);

void board_sample_timer_start(uint32_t rate_hz);
void board_sample_timer_stop(void);
void board_watchdog_refresh(void);
void board_enter_stop_mode(void);

bool board_dac_dma_start(const uint16_t *samples, size_t count);

#endif
