#ifndef APP_TYPES_H
#define APP_TYPES_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

typedef enum {
    MODE_AUTO_SAMPLE = 0,
    MODE_MANUAL_SAMPLE,
    MODE_REPLAY,
    MODE_STANDBY,
    MODE_COUNT
} WorkMode;

typedef struct {
    uint32_t sample_rate_hz;
    uint16_t alarm_threshold_mv;
    uint8_t filter_window;
} AppConfig;

typedef struct {
    uint32_t timestamp_ms;
    uint16_t raw;
    uint16_t filtered;
    uint16_t millivolts;
} SampleRecord;

#endif
